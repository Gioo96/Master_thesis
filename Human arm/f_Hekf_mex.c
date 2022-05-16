/* This file was automatically generated by CasADi.
   The CasADi copyright holders make no ownership claim of its contents. */
#ifdef __cplusplus
extern "C" {
#endif

/* How to prefix internal symbols */
#ifdef CASADI_CODEGEN_PREFIX
  #define CASADI_NAMESPACE_CONCAT(NS, ID) _CASADI_NAMESPACE_CONCAT(NS, ID)
  #define _CASADI_NAMESPACE_CONCAT(NS, ID) NS ## ID
  #define CASADI_PREFIX(ID) CASADI_NAMESPACE_CONCAT(CODEGEN_PREFIX, ID)
#else
  #define CASADI_PREFIX(ID) f_Hekf_mex_ ## ID
#endif

#include <math.h>
#include <stdio.h>
#include <string.h>
#ifdef MATLAB_MEX_FILE
#include <mex.h>
#endif

#ifndef casadi_real
#define casadi_real double
#endif

#ifndef casadi_int
#define casadi_int long long int
#endif

/* Add prefix to internal symbols */
#define casadi_f0 CASADI_PREFIX(f0)
#define casadi_fill CASADI_PREFIX(fill)
#define casadi_from_mex CASADI_PREFIX(from_mex)
#define casadi_s0 CASADI_PREFIX(s0)
#define casadi_s1 CASADI_PREFIX(s1)
#define casadi_to_mex CASADI_PREFIX(to_mex)

/* Symbol visibility in DLLs */
#ifndef CASADI_SYMBOL_EXPORT
  #if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    #if defined(STATIC_LINKED)
      #define CASADI_SYMBOL_EXPORT
    #else
      #define CASADI_SYMBOL_EXPORT __declspec(dllexport)
    #endif
  #elif defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
    #define CASADI_SYMBOL_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define CASADI_SYMBOL_EXPORT
  #endif
#endif

void casadi_fill(casadi_real* x, casadi_int n, casadi_real alpha) {
  casadi_int i;
  if (x) {
    for (i=0; i<n; ++i) *x++ = alpha;
  }
}

#ifdef MATLAB_MEX_FILE
casadi_real* casadi_from_mex(const mxArray* p, casadi_real* y, const casadi_int* sp, casadi_real* w) {
  casadi_int nrow, ncol, is_sparse, c, k, p_nrow, p_ncol;
  const casadi_int *colind, *row;
  mwIndex *Jc, *Ir;
  const double* p_data;
  if (!mxIsDouble(p) || mxGetNumberOfDimensions(p)!=2)
    mexErrMsgIdAndTxt("Casadi:RuntimeError",
      "\"from_mex\" failed: Not a two-dimensional matrix of double precision.");
  nrow = *sp++;
  ncol = *sp++;
  colind = sp;
  row = sp+ncol+1;
  p_nrow = mxGetM(p);
  p_ncol = mxGetN(p);
  is_sparse = mxIsSparse(p);
  Jc = 0;
  Ir = 0;
  if (is_sparse) {
    Jc = mxGetJc(p);
    Ir = mxGetIr(p);
  }
  p_data = (const double*)mxGetData(p);
  if (p_nrow==1 && p_ncol==1) {
    casadi_int nnz;
    double v = is_sparse && Jc[1]==0 ? 0 : *p_data;
    nnz = sp[ncol];
    casadi_fill(y, nnz, v);
  } else {
    casadi_int tr = 0;
    if (nrow!=p_nrow || ncol!=p_ncol) {
      tr = nrow==p_ncol && ncol==p_nrow && (nrow==1 || ncol==1);
      if (!tr) mexErrMsgIdAndTxt("Casadi:RuntimeError",
                                 "\"from_mex\" failed: Dimension mismatch. "
                                 "Expected %d-by-%d, got %d-by-%d instead.",
                                 nrow, ncol, p_nrow, p_ncol);
    }
    if (is_sparse) {
      if (tr) {
        for (c=0; c<ncol; ++c)
          for (k=colind[c]; k<colind[c+1]; ++k) w[row[k]+c*nrow]=0;
        for (c=0; c<p_ncol; ++c)
          for (k=Jc[c]; k<(casadi_int) Jc[c+1]; ++k) w[c+Ir[k]*p_ncol] = p_data[k];
        for (c=0; c<ncol; ++c)
          for (k=colind[c]; k<colind[c+1]; ++k) y[k] = w[row[k]+c*nrow];
      } else {
        for (c=0; c<ncol; ++c) {
          for (k=colind[c]; k<colind[c+1]; ++k) w[row[k]]=0;
          for (k=Jc[c]; k<(casadi_int) Jc[c+1]; ++k) w[Ir[k]]=p_data[k];
          for (k=colind[c]; k<colind[c+1]; ++k) y[k]=w[row[k]];
        }
      }
    } else {
      for (c=0; c<ncol; ++c) {
        for (k=colind[c]; k<colind[c+1]; ++k) {
          y[k] = p_data[row[k]+c*nrow];
        }
      }
    }
  }
  return y;
}

#endif

#define casadi_to_double(x) ((double) x)

#ifdef MATLAB_MEX_FILE
mxArray* casadi_to_mex(const casadi_int* sp, const casadi_real* x) {
  casadi_int nrow, ncol, c, k;
#ifndef CASADI_MEX_NO_SPARSE
  casadi_int nnz;
#endif
  const casadi_int *colind, *row;
  mxArray *p;
  double *d;
#ifndef CASADI_MEX_NO_SPARSE
  casadi_int i;
  mwIndex *j;
#endif /* CASADI_MEX_NO_SPARSE */
  nrow = *sp++;
  ncol = *sp++;
  colind = sp;
  row = sp+ncol+1;
#ifndef CASADI_MEX_NO_SPARSE
  nnz = sp[ncol];
  if (nnz!=nrow*ncol) {
    p = mxCreateSparse(nrow, ncol, nnz, mxREAL);
    for (i=0, j=mxGetJc(p); i<=ncol; ++i) *j++ = *colind++;
    for (i=0, j=mxGetIr(p); i<nnz; ++i) *j++ = *row++;
    if (x) {
      d = (double*)mxGetData(p);
      for (i=0; i<nnz; ++i) *d++ = casadi_to_double(*x++);
    }
    return p;
  }
#endif /* CASADI_MEX_NO_SPARSE */
  p = mxCreateDoubleMatrix(nrow, ncol, mxREAL);
  if (x) {
    d = (double*)mxGetData(p);
    for (c=0; c<ncol; ++c) {
      for (k=colind[c]; k<colind[c+1]; ++k) {
        d[row[k]+c*nrow] = casadi_to_double(*x++);
      }
    }
  }
  return p;
}

#endif

#ifndef CASADI_PRINTF
#ifdef MATLAB_MEX_FILE
  #define CASADI_PRINTF mexPrintf
#else
  #define CASADI_PRINTF printf
#endif
#endif

static const casadi_int casadi_s0[11] = {7, 1, 0, 7, 0, 1, 2, 3, 4, 5, 6};
static const casadi_int casadi_s1[115] = {27, 7, 0, 18, 45, 72, 87, 93, 99, 105, 1, 2, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 19, 20, 22, 23, 25, 26, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 21, 22, 23, 24, 25, 26, 21, 22, 23, 24, 25, 26, 21, 22, 23, 24, 25, 26};

/* f_J:(i0[7])->(o0[27x7,105nz]) */
static int casadi_f0(const casadi_real** arg, casadi_real** res, casadi_int* iw, casadi_real* w, int mem) {
  casadi_real a0, a1, a10, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a11, a110, a111, a112, a113, a114, a12, a13, a14, a15, a16, a17, a18, a19, a2, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a3, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a4, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a5, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a6, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a7, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a8, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a9, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99;
  a0=3.8769999999999999e-02;
  a1=arg[0]? arg[0][2] : 0;
  a2=cos(a1);
  a3=arg[0]? arg[0][1] : 0;
  a4=sin(a3);
  a5=arg[0]? arg[0][0] : 0;
  a6=cos(a5);
  a6=(a4*a6);
  a7=(a2*a6);
  a8=sin(a1);
  a9=sin(a5);
  a10=(a8*a9);
  a7=(a7-a10);
  a7=(a0*a7);
  a10=-3.5870999999999997e-01;
  a11=cos(a1);
  a9=(a11*a9);
  a12=sin(a1);
  a6=(a12*a6);
  a9=(a9+a6);
  a9=(a10*a9);
  a7=(a7-a9);
  if (res[0]!=0) res[0][0]=a7;
  a7=cos(a5);
  a8=(a8*a7);
  a9=sin(a5);
  a9=(a4*a9);
  a6=(a2*a9);
  a8=(a8+a6);
  a8=(a0*a8);
  a11=(a11*a7);
  a9=(a12*a9);
  a11=(a11-a9);
  a11=(a10*a11);
  a8=(a8+a11);
  if (res[0]!=0) res[0][1]=a8;
  a8=-3.8769999999999999e-02;
  a11=cos(a1);
  a9=sin(a3);
  a7=cos(a5);
  a7=(a9*a7);
  a6=(a11*a7);
  a13=sin(a1);
  a14=sin(a5);
  a15=(a13*a14);
  a6=(a6-a15);
  a6=(a8*a6);
  a15=cos(a1);
  a14=(a15*a14);
  a16=sin(a1);
  a7=(a16*a7);
  a14=(a14+a7);
  a14=(a10*a14);
  a6=(a6-a14);
  if (res[0]!=0) res[0][2]=a6;
  a6=cos(a5);
  a13=(a13*a6);
  a14=sin(a5);
  a14=(a9*a14);
  a7=(a11*a14);
  a13=(a13+a7);
  a13=(a8*a13);
  a15=(a15*a6);
  a14=(a16*a14);
  a15=(a15-a14);
  a15=(a10*a15);
  a13=(a13+a15);
  if (res[0]!=0) res[0][3]=a13;
  a13=cos(a1);
  a15=sin(a3);
  a14=cos(a5);
  a14=(a15*a14);
  a6=(a13*a14);
  a7=sin(a1);
  a17=sin(a5);
  a18=(a7*a17);
  a6=(a6-a18);
  a6=(a0*a6);
  a18=-1.1957000000000000e-01;
  a19=cos(a1);
  a17=(a19*a17);
  a20=sin(a1);
  a14=(a20*a14);
  a17=(a17+a14);
  a17=(a18*a17);
  a6=(a6-a17);
  if (res[0]!=0) res[0][4]=a6;
  a6=cos(a5);
  a7=(a7*a6);
  a17=sin(a5);
  a17=(a15*a17);
  a14=(a13*a17);
  a7=(a7+a14);
  a7=(a0*a7);
  a19=(a19*a6);
  a17=(a20*a17);
  a19=(a19-a17);
  a19=(a18*a19);
  a7=(a7+a19);
  if (res[0]!=0) res[0][5]=a7;
  a7=cos(a1);
  a19=sin(a3);
  a17=cos(a5);
  a17=(a19*a17);
  a6=(a7*a17);
  a14=sin(a1);
  a21=sin(a5);
  a22=(a14*a21);
  a6=(a6-a22);
  a6=(a8*a6);
  a22=cos(a1);
  a21=(a22*a21);
  a23=sin(a1);
  a17=(a23*a17);
  a21=(a21+a17);
  a21=(a18*a21);
  a6=(a6-a21);
  if (res[0]!=0) res[0][6]=a6;
  a6=cos(a5);
  a14=(a14*a6);
  a21=sin(a5);
  a21=(a19*a21);
  a17=(a7*a21);
  a14=(a14+a17);
  a14=(a8*a14);
  a22=(a22*a6);
  a21=(a23*a21);
  a22=(a22-a21);
  a22=(a18*a22);
  a14=(a14+a22);
  if (res[0]!=0) res[0][7]=a14;
  a14=3.2802999999999999e-02;
  a22=cos(a1);
  a21=sin(a3);
  a6=cos(a5);
  a17=(a21*a6);
  a24=(a22*a17);
  a25=sin(a1);
  a26=sin(a5);
  a27=(a25*a26);
  a24=(a24-a27);
  a24=(a14*a24);
  a27=-1.9380000000000003e-01;
  a28=arg[0]? arg[0][3] : 0;
  a29=cos(a28);
  a29=(a27*a29);
  a30=cos(a1);
  a26=(a30*a26);
  a31=sin(a1);
  a17=(a31*a17);
  a26=(a26+a17);
  a17=(a29*a26);
  a24=(a24-a17);
  a17=sin(a28);
  a17=(a27*a17);
  a32=cos(a3);
  a6=(a32*a6);
  a6=(a17*a6);
  a24=(a24-a6);
  a6=-4.7827999999999998e-01;
  a26=(a6*a26);
  a24=(a24-a26);
  if (res[0]!=0) res[0][8]=a24;
  a24=cos(a5);
  a25=(a25*a24);
  a26=sin(a5);
  a33=(a21*a26);
  a34=(a22*a33);
  a25=(a25+a34);
  a25=(a14*a25);
  a24=(a30*a24);
  a33=(a31*a33);
  a24=(a24-a33);
  a33=(a29*a24);
  a25=(a25+a33);
  a26=(a32*a26);
  a26=(a17*a26);
  a25=(a25-a26);
  a24=(a6*a24);
  a25=(a25+a24);
  if (res[0]!=0) res[0][9]=a25;
  a25=-3.2802999999999999e-02;
  a24=cos(a1);
  a26=sin(a3);
  a33=cos(a5);
  a34=(a26*a33);
  a35=(a24*a34);
  a36=sin(a1);
  a37=sin(a5);
  a38=(a36*a37);
  a35=(a35-a38);
  a35=(a25*a35);
  a38=-1.2920000000000001e-01;
  a39=cos(a28);
  a39=(a38*a39);
  a40=cos(a1);
  a37=(a40*a37);
  a41=sin(a1);
  a34=(a41*a34);
  a37=(a37+a34);
  a34=(a39*a37);
  a35=(a35-a34);
  a34=sin(a28);
  a34=(a38*a34);
  a42=cos(a3);
  a33=(a42*a33);
  a33=(a34*a33);
  a35=(a35-a33);
  a37=(a6*a37);
  a35=(a35-a37);
  if (res[0]!=0) res[0][10]=a35;
  a35=cos(a5);
  a36=(a36*a35);
  a37=sin(a5);
  a33=(a26*a37);
  a43=(a24*a33);
  a36=(a36+a43);
  a36=(a25*a36);
  a35=(a40*a35);
  a33=(a41*a33);
  a35=(a35-a33);
  a33=(a39*a35);
  a36=(a36+a33);
  a37=(a42*a37);
  a37=(a34*a37);
  a36=(a36-a37);
  a35=(a6*a35);
  a36=(a36+a35);
  if (res[0]!=0) res[0][11]=a36;
  a36=cos(a1);
  a35=sin(a3);
  a37=cos(a5);
  a33=(a35*a37);
  a43=(a36*a33);
  a44=sin(a1);
  a45=sin(a5);
  a46=(a44*a45);
  a43=(a43-a46);
  a43=(a14*a43);
  a46=-6.4600000000000005e-02;
  a47=cos(a28);
  a47=(a46*a47);
  a48=cos(a1);
  a45=(a48*a45);
  a49=sin(a1);
  a33=(a49*a33);
  a45=(a45+a33);
  a33=(a47*a45);
  a43=(a43-a33);
  a33=sin(a28);
  a33=(a46*a33);
  a50=cos(a3);
  a37=(a50*a37);
  a37=(a33*a37);
  a43=(a43-a37);
  a45=(a6*a45);
  a43=(a43-a45);
  if (res[0]!=0) res[0][12]=a43;
  a43=cos(a5);
  a44=(a44*a43);
  a45=sin(a5);
  a37=(a35*a45);
  a51=(a36*a37);
  a44=(a44+a51);
  a44=(a14*a44);
  a43=(a48*a43);
  a37=(a49*a37);
  a43=(a43-a37);
  a37=(a47*a43);
  a44=(a44+a37);
  a45=(a50*a45);
  a45=(a33*a45);
  a44=(a44-a45);
  a43=(a6*a43);
  a44=(a44+a43);
  if (res[0]!=0) res[0][13]=a44;
  a44=3.2803499999999999e-02;
  a43=arg[0]? arg[0][5] : 0;
  a45=sin(a43);
  a45=(a44*a45);
  a37=-1.4530499999999999e-01;
  a51=cos(a43);
  a52=arg[0]? arg[0][6] : 0;
  a53=sin(a52);
  a54=(a51*a53);
  a54=(a37*a54);
  a45=(a45-a54);
  a54=cos(a1);
  a55=sin(a3);
  a56=cos(a5);
  a57=(a55*a56);
  a58=(a54*a57);
  a59=sin(a1);
  a60=sin(a5);
  a61=(a59*a60);
  a58=(a58-a61);
  a58=(a45*a58);
  a61=arg[0]? arg[0][4] : 0;
  a62=cos(a61);
  a63=cos(a52);
  a64=(a62*a63);
  a65=sin(a61);
  a66=sin(a43);
  a67=(a65*a66);
  a68=(a67*a53);
  a64=(a64-a68);
  a64=(a37*a64);
  a68=cos(a43);
  a69=(a65*a68);
  a69=(a44*a69);
  a64=(a64-a69);
  a69=cos(a28);
  a70=cos(a1);
  a60=(a70*a60);
  a71=sin(a1);
  a57=(a71*a57);
  a60=(a60+a57);
  a57=(a69*a60);
  a72=sin(a28);
  a73=cos(a3);
  a56=(a73*a56);
  a74=(a72*a56);
  a57=(a57+a74);
  a57=(a64*a57);
  a58=(a58-a57);
  a57=cos(a61);
  a74=(a57*a66);
  a75=(a74*a53);
  a76=sin(a61);
  a77=(a76*a63);
  a75=(a75+a77);
  a75=(a37*a75);
  a77=(a57*a68);
  a77=(a44*a77);
  a75=(a75+a77);
  a77=cos(a28);
  a78=(a77*a56);
  a79=sin(a28);
  a80=(a79*a60);
  a78=(a78-a80);
  a78=(a75*a78);
  a58=(a58-a78);
  a78=-2.5840000000000002e-01;
  a80=(a78*a69);
  a81=(a80*a60);
  a82=(a78*a72);
  a56=(a82*a56);
  a81=(a81+a56);
  a60=(a6*a60);
  a81=(a81+a60);
  a58=(a58-a81);
  if (res[0]!=0) res[0][14]=a58;
  a58=cos(a5);
  a81=(a59*a58);
  a60=sin(a5);
  a56=(a55*a60);
  a83=(a54*a56);
  a81=(a81+a83);
  a81=(a45*a81);
  a58=(a70*a58);
  a56=(a71*a56);
  a58=(a58-a56);
  a56=(a69*a58);
  a60=(a73*a60);
  a83=(a72*a60);
  a56=(a56-a83);
  a56=(a64*a56);
  a81=(a81+a56);
  a56=(a77*a60);
  a83=(a79*a58);
  a56=(a56+a83);
  a56=(a75*a56);
  a81=(a81-a56);
  a56=(a80*a58);
  a60=(a82*a60);
  a56=(a56-a60);
  a58=(a6*a58);
  a56=(a56+a58);
  a81=(a81+a56);
  if (res[0]!=0) res[0][15]=a81;
  a81=sin(a43);
  a81=(a44*a81);
  a56=-4.8434999999999999e-02;
  a58=cos(a43);
  a60=sin(a52);
  a83=(a58*a60);
  a83=(a56*a83);
  a81=(a81-a83);
  a83=cos(a1);
  a84=sin(a3);
  a85=cos(a5);
  a86=(a84*a85);
  a87=(a83*a86);
  a88=sin(a1);
  a89=sin(a5);
  a90=(a88*a89);
  a87=(a87-a90);
  a87=(a81*a87);
  a90=cos(a61);
  a91=cos(a52);
  a92=(a90*a91);
  a93=sin(a61);
  a94=sin(a43);
  a95=(a93*a94);
  a96=(a95*a60);
  a92=(a92-a96);
  a92=(a56*a92);
  a96=cos(a43);
  a97=(a93*a96);
  a97=(a44*a97);
  a92=(a92-a97);
  a97=cos(a28);
  a98=cos(a1);
  a89=(a98*a89);
  a99=sin(a1);
  a86=(a99*a86);
  a89=(a89+a86);
  a86=(a97*a89);
  a100=sin(a28);
  a101=cos(a3);
  a85=(a101*a85);
  a102=(a100*a85);
  a86=(a86+a102);
  a86=(a92*a86);
  a87=(a87-a86);
  a86=cos(a61);
  a102=(a86*a94);
  a103=(a102*a60);
  a104=sin(a61);
  a105=(a104*a91);
  a103=(a103+a105);
  a103=(a56*a103);
  a105=(a86*a96);
  a105=(a44*a105);
  a103=(a103+a105);
  a105=cos(a28);
  a106=(a105*a85);
  a107=sin(a28);
  a108=(a107*a89);
  a106=(a106-a108);
  a106=(a103*a106);
  a87=(a87-a106);
  a106=(a78*a97);
  a108=(a106*a89);
  a109=(a78*a100);
  a85=(a109*a85);
  a108=(a108+a85);
  a89=(a6*a89);
  a108=(a108+a89);
  a87=(a87-a108);
  if (res[0]!=0) res[0][16]=a87;
  a87=cos(a5);
  a108=(a88*a87);
  a89=sin(a5);
  a85=(a84*a89);
  a110=(a83*a85);
  a108=(a108+a110);
  a108=(a81*a108);
  a87=(a98*a87);
  a85=(a99*a85);
  a87=(a87-a85);
  a85=(a97*a87);
  a89=(a101*a89);
  a110=(a100*a89);
  a85=(a85-a110);
  a85=(a92*a85);
  a108=(a108+a85);
  a85=(a105*a89);
  a110=(a107*a87);
  a85=(a85+a110);
  a85=(a103*a85);
  a108=(a108-a85);
  a85=(a106*a87);
  a89=(a109*a89);
  a85=(a85-a89);
  a87=(a6*a87);
  a85=(a85+a87);
  a108=(a108+a85);
  if (res[0]!=0) res[0][17]=a108;
  a108=sin(a3);
  a85=(a12*a108);
  a85=(a10*a85);
  a108=(a2*a108);
  a108=(a0*a108);
  a85=(a85-a108);
  if (res[0]!=0) res[0][18]=a85;
  a85=sin(a5);
  a108=cos(a3);
  a87=(a85*a108);
  a89=(a2*a87);
  a89=(a0*a89);
  a87=(a12*a87);
  a87=(a10*a87);
  a89=(a89-a87);
  if (res[0]!=0) res[0][19]=a89;
  a89=cos(a5);
  a108=(a89*a108);
  a12=(a12*a108);
  a12=(a10*a12);
  a2=(a2*a108);
  a2=(a0*a2);
  a12=(a12-a2);
  if (res[0]!=0) res[0][20]=a12;
  a12=sin(a3);
  a2=(a16*a12);
  a2=(a10*a2);
  a12=(a11*a12);
  a12=(a8*a12);
  a2=(a2-a12);
  if (res[0]!=0) res[0][21]=a2;
  a2=sin(a5);
  a12=cos(a3);
  a108=(a2*a12);
  a87=(a11*a108);
  a87=(a8*a87);
  a108=(a16*a108);
  a108=(a10*a108);
  a87=(a87-a108);
  if (res[0]!=0) res[0][22]=a87;
  a87=cos(a5);
  a12=(a87*a12);
  a16=(a16*a12);
  a16=(a10*a16);
  a11=(a11*a12);
  a11=(a8*a11);
  a16=(a16-a11);
  if (res[0]!=0) res[0][23]=a16;
  a16=sin(a3);
  a11=(a20*a16);
  a11=(a18*a11);
  a16=(a13*a16);
  a16=(a0*a16);
  a11=(a11-a16);
  if (res[0]!=0) res[0][24]=a11;
  a11=sin(a5);
  a16=cos(a3);
  a12=(a11*a16);
  a108=(a13*a12);
  a108=(a0*a108);
  a12=(a20*a12);
  a12=(a18*a12);
  a108=(a108-a12);
  if (res[0]!=0) res[0][25]=a108;
  a108=cos(a5);
  a16=(a108*a16);
  a20=(a20*a16);
  a20=(a18*a20);
  a13=(a13*a16);
  a13=(a0*a13);
  a20=(a20-a13);
  if (res[0]!=0) res[0][26]=a20;
  a20=sin(a3);
  a13=(a23*a20);
  a13=(a18*a13);
  a20=(a7*a20);
  a20=(a8*a20);
  a13=(a13-a20);
  if (res[0]!=0) res[0][27]=a13;
  a13=sin(a5);
  a20=cos(a3);
  a16=(a13*a20);
  a12=(a7*a16);
  a12=(a8*a12);
  a16=(a23*a16);
  a16=(a18*a16);
  a12=(a12-a16);
  if (res[0]!=0) res[0][28]=a12;
  a12=cos(a5);
  a20=(a12*a20);
  a23=(a23*a20);
  a23=(a18*a23);
  a7=(a7*a20);
  a7=(a8*a7);
  a23=(a23-a7);
  if (res[0]!=0) res[0][29]=a23;
  a23=sin(a3);
  a7=(a31*a23);
  a20=(a29*a7);
  a23=(a22*a23);
  a23=(a14*a23);
  a20=(a20-a23);
  a23=cos(a3);
  a23=(a17*a23);
  a20=(a20+a23);
  a7=(a6*a7);
  a20=(a20+a7);
  if (res[0]!=0) res[0][30]=a20;
  a20=sin(a5);
  a7=cos(a3);
  a23=(a20*a7);
  a16=(a22*a23);
  a16=(a14*a16);
  a23=(a31*a23);
  a110=(a29*a23);
  a16=(a16-a110);
  a110=sin(a3);
  a111=(a20*a110);
  a111=(a17*a111);
  a16=(a16+a111);
  a23=(a6*a23);
  a16=(a16-a23);
  if (res[0]!=0) res[0][31]=a16;
  a16=cos(a5);
  a7=(a16*a7);
  a23=(a31*a7);
  a111=(a29*a23);
  a22=(a22*a7);
  a22=(a14*a22);
  a111=(a111-a22);
  a110=(a16*a110);
  a17=(a17*a110);
  a111=(a111-a17);
  a23=(a6*a23);
  a111=(a111+a23);
  if (res[0]!=0) res[0][32]=a111;
  a111=sin(a3);
  a23=(a41*a111);
  a17=(a39*a23);
  a111=(a24*a111);
  a111=(a25*a111);
  a17=(a17-a111);
  a111=cos(a3);
  a111=(a34*a111);
  a17=(a17+a111);
  a23=(a6*a23);
  a17=(a17+a23);
  if (res[0]!=0) res[0][33]=a17;
  a17=sin(a5);
  a23=cos(a3);
  a111=(a17*a23);
  a110=(a24*a111);
  a110=(a25*a110);
  a111=(a41*a111);
  a22=(a39*a111);
  a110=(a110-a22);
  a22=sin(a3);
  a7=(a17*a22);
  a7=(a34*a7);
  a110=(a110+a7);
  a111=(a6*a111);
  a110=(a110-a111);
  if (res[0]!=0) res[0][34]=a110;
  a110=cos(a5);
  a23=(a110*a23);
  a111=(a41*a23);
  a7=(a39*a111);
  a24=(a24*a23);
  a24=(a25*a24);
  a7=(a7-a24);
  a22=(a110*a22);
  a34=(a34*a22);
  a7=(a7-a34);
  a111=(a6*a111);
  a7=(a7+a111);
  if (res[0]!=0) res[0][35]=a7;
  a7=sin(a3);
  a111=(a49*a7);
  a34=(a47*a111);
  a7=(a36*a7);
  a7=(a14*a7);
  a34=(a34-a7);
  a7=cos(a3);
  a7=(a33*a7);
  a34=(a34+a7);
  a111=(a6*a111);
  a34=(a34+a111);
  if (res[0]!=0) res[0][36]=a34;
  a34=sin(a5);
  a111=cos(a3);
  a7=(a34*a111);
  a22=(a36*a7);
  a22=(a14*a22);
  a7=(a49*a7);
  a24=(a47*a7);
  a22=(a22-a24);
  a24=sin(a3);
  a23=(a34*a24);
  a23=(a33*a23);
  a22=(a22+a23);
  a7=(a6*a7);
  a22=(a22-a7);
  if (res[0]!=0) res[0][37]=a22;
  a22=cos(a5);
  a111=(a22*a111);
  a7=(a49*a111);
  a23=(a47*a7);
  a36=(a36*a111);
  a36=(a14*a36);
  a23=(a23-a36);
  a24=(a22*a24);
  a33=(a33*a24);
  a23=(a23-a33);
  a7=(a6*a7);
  a23=(a23+a7);
  if (res[0]!=0) res[0][38]=a23;
  a23=cos(a3);
  a7=(a72*a23);
  a33=sin(a3);
  a24=(a71*a33);
  a36=(a69*a24);
  a7=(a7+a36);
  a7=(a64*a7);
  a33=(a54*a33);
  a33=(a45*a33);
  a7=(a7-a33);
  a33=(a77*a23);
  a36=(a79*a24);
  a33=(a33-a36);
  a33=(a75*a33);
  a7=(a7+a33);
  a23=(a82*a23);
  a33=(a80*a24);
  a23=(a23+a33);
  a24=(a6*a24);
  a23=(a23+a24);
  a7=(a7+a23);
  if (res[0]!=0) res[0][39]=a7;
  a7=sin(a5);
  a23=cos(a3);
  a24=(a7*a23);
  a33=(a54*a24);
  a33=(a45*a33);
  a36=sin(a3);
  a111=(a7*a36);
  a112=(a72*a111);
  a24=(a71*a24);
  a113=(a69*a24);
  a112=(a112-a113);
  a112=(a64*a112);
  a33=(a33+a112);
  a112=(a79*a24);
  a113=(a77*a111);
  a112=(a112+a113);
  a112=(a75*a112);
  a33=(a33+a112);
  a111=(a82*a111);
  a112=(a80*a24);
  a111=(a111-a112);
  a24=(a6*a24);
  a111=(a111-a24);
  a33=(a33+a111);
  if (res[0]!=0) res[0][40]=a33;
  a33=cos(a5);
  a23=(a33*a23);
  a111=(a71*a23);
  a24=(a69*a111);
  a36=(a33*a36);
  a112=(a72*a36);
  a24=(a24-a112);
  a24=(a64*a24);
  a23=(a54*a23);
  a23=(a45*a23);
  a24=(a24-a23);
  a23=(a77*a36);
  a112=(a79*a111);
  a23=(a23+a112);
  a23=(a75*a23);
  a24=(a24-a23);
  a23=(a80*a111);
  a82=(a82*a36);
  a23=(a23-a82);
  a111=(a6*a111);
  a23=(a23+a111);
  a24=(a24+a23);
  if (res[0]!=0) res[0][41]=a24;
  a24=cos(a3);
  a23=(a100*a24);
  a111=sin(a3);
  a82=(a99*a111);
  a36=(a97*a82);
  a23=(a23+a36);
  a23=(a92*a23);
  a111=(a83*a111);
  a111=(a81*a111);
  a23=(a23-a111);
  a111=(a105*a24);
  a36=(a107*a82);
  a111=(a111-a36);
  a111=(a103*a111);
  a23=(a23+a111);
  a24=(a109*a24);
  a111=(a106*a82);
  a24=(a24+a111);
  a82=(a6*a82);
  a24=(a24+a82);
  a23=(a23+a24);
  if (res[0]!=0) res[0][42]=a23;
  a23=sin(a5);
  a24=cos(a3);
  a82=(a23*a24);
  a111=(a83*a82);
  a111=(a81*a111);
  a36=sin(a3);
  a112=(a23*a36);
  a113=(a100*a112);
  a82=(a99*a82);
  a114=(a97*a82);
  a113=(a113-a114);
  a113=(a92*a113);
  a111=(a111+a113);
  a113=(a107*a82);
  a114=(a105*a112);
  a113=(a113+a114);
  a113=(a103*a113);
  a111=(a111+a113);
  a112=(a109*a112);
  a113=(a106*a82);
  a112=(a112-a113);
  a82=(a6*a82);
  a112=(a112-a82);
  a111=(a111+a112);
  if (res[0]!=0) res[0][43]=a111;
  a111=cos(a5);
  a24=(a111*a24);
  a112=(a99*a24);
  a82=(a97*a112);
  a36=(a111*a36);
  a113=(a100*a36);
  a82=(a82-a113);
  a82=(a92*a82);
  a24=(a83*a24);
  a24=(a81*a24);
  a82=(a82-a24);
  a24=(a105*a36);
  a113=(a107*a112);
  a24=(a24+a113);
  a24=(a103*a24);
  a82=(a82-a24);
  a24=(a106*a112);
  a109=(a109*a36);
  a24=(a24-a109);
  a112=(a6*a112);
  a24=(a24+a112);
  a82=(a82+a24);
  if (res[0]!=0) res[0][44]=a82;
  a82=cos(a3);
  a24=sin(a1);
  a112=(a82*a24);
  a112=(a0*a112);
  a109=cos(a1);
  a82=(a82*a109);
  a82=(a10*a82);
  a112=(a112+a82);
  a112=(-a112);
  if (res[0]!=0) res[0][45]=a112;
  a112=cos(a5);
  a82=cos(a1);
  a36=(a112*a82);
  a85=(a85*a4);
  a113=(a85*a24);
  a36=(a36-a113);
  a36=(a0*a36);
  a113=sin(a1);
  a112=(a112*a113);
  a85=(a85*a109);
  a112=(a112+a85);
  a112=(a10*a112);
  a36=(a36-a112);
  if (res[0]!=0) res[0][46]=a36;
  a36=sin(a5);
  a82=(a36*a82);
  a89=(a89*a4);
  a24=(a89*a24);
  a82=(a82+a24);
  a82=(a0*a82);
  a89=(a89*a109);
  a36=(a36*a113);
  a89=(a89-a36);
  a89=(a10*a89);
  a82=(a82+a89);
  if (res[0]!=0) res[0][47]=a82;
  a82=cos(a3);
  a89=sin(a1);
  a36=(a82*a89);
  a36=(a8*a36);
  a113=cos(a1);
  a82=(a82*a113);
  a82=(a10*a82);
  a36=(a36+a82);
  a36=(-a36);
  if (res[0]!=0) res[0][48]=a36;
  a36=cos(a5);
  a82=cos(a1);
  a109=(a36*a82);
  a2=(a2*a9);
  a24=(a2*a89);
  a109=(a109-a24);
  a109=(a8*a109);
  a24=sin(a1);
  a36=(a36*a24);
  a2=(a2*a113);
  a36=(a36+a2);
  a36=(a10*a36);
  a109=(a109-a36);
  if (res[0]!=0) res[0][49]=a109;
  a109=sin(a5);
  a82=(a109*a82);
  a87=(a87*a9);
  a89=(a87*a89);
  a82=(a82+a89);
  a82=(a8*a82);
  a87=(a87*a113);
  a109=(a109*a24);
  a87=(a87-a109);
  a10=(a10*a87);
  a82=(a82+a10);
  if (res[0]!=0) res[0][50]=a82;
  a82=cos(a3);
  a10=sin(a1);
  a87=(a82*a10);
  a87=(a0*a87);
  a109=cos(a1);
  a82=(a82*a109);
  a82=(a18*a82);
  a87=(a87+a82);
  a87=(-a87);
  if (res[0]!=0) res[0][51]=a87;
  a87=cos(a5);
  a82=cos(a1);
  a24=(a87*a82);
  a11=(a11*a15);
  a113=(a11*a10);
  a24=(a24-a113);
  a24=(a0*a24);
  a113=sin(a1);
  a87=(a87*a113);
  a11=(a11*a109);
  a87=(a87+a11);
  a87=(a18*a87);
  a24=(a24-a87);
  if (res[0]!=0) res[0][52]=a24;
  a24=sin(a5);
  a82=(a24*a82);
  a108=(a108*a15);
  a10=(a108*a10);
  a82=(a82+a10);
  a0=(a0*a82);
  a108=(a108*a109);
  a24=(a24*a113);
  a108=(a108-a24);
  a108=(a18*a108);
  a0=(a0+a108);
  if (res[0]!=0) res[0][53]=a0;
  a0=cos(a3);
  a108=sin(a1);
  a24=(a0*a108);
  a24=(a8*a24);
  a113=cos(a1);
  a0=(a0*a113);
  a0=(a18*a0);
  a24=(a24+a0);
  a24=(-a24);
  if (res[0]!=0) res[0][54]=a24;
  a24=cos(a5);
  a0=cos(a1);
  a109=(a24*a0);
  a13=(a13*a19);
  a82=(a13*a108);
  a109=(a109-a82);
  a109=(a8*a109);
  a82=sin(a1);
  a24=(a24*a82);
  a13=(a13*a113);
  a24=(a24+a13);
  a24=(a18*a24);
  a109=(a109-a24);
  if (res[0]!=0) res[0][55]=a109;
  a109=sin(a5);
  a0=(a109*a0);
  a12=(a12*a19);
  a108=(a12*a108);
  a0=(a0+a108);
  a8=(a8*a0);
  a12=(a12*a113);
  a109=(a109*a82);
  a12=(a12-a109);
  a18=(a18*a12);
  a8=(a8+a18);
  if (res[0]!=0) res[0][56]=a8;
  a8=cos(a3);
  a18=sin(a1);
  a12=(a8*a18);
  a12=(a14*a12);
  a109=cos(a1);
  a82=(a8*a109);
  a113=(a29*a82);
  a12=(a12+a113);
  a82=(a6*a82);
  a12=(a12+a82);
  a12=(-a12);
  if (res[0]!=0) res[0][57]=a12;
  a12=cos(a5);
  a82=cos(a1);
  a113=(a12*a82);
  a0=(a20*a21);
  a108=(a0*a18);
  a113=(a113-a108);
  a113=(a14*a113);
  a108=sin(a1);
  a19=(a12*a108);
  a24=(a0*a109);
  a19=(a19+a24);
  a24=(a29*a19);
  a113=(a113-a24);
  a19=(a6*a19);
  a113=(a113-a19);
  if (res[0]!=0) res[0][58]=a113;
  a113=sin(a5);
  a82=(a113*a82);
  a21=(a16*a21);
  a18=(a21*a18);
  a82=(a82+a18);
  a82=(a14*a82);
  a109=(a21*a109);
  a108=(a113*a108);
  a109=(a109-a108);
  a29=(a29*a109);
  a82=(a82+a29);
  a109=(a6*a109);
  a82=(a82+a109);
  if (res[0]!=0) res[0][59]=a82;
  a82=cos(a3);
  a109=sin(a1);
  a29=(a82*a109);
  a29=(a25*a29);
  a108=cos(a1);
  a18=(a82*a108);
  a19=(a39*a18);
  a29=(a29+a19);
  a18=(a6*a18);
  a29=(a29+a18);
  a29=(-a29);
  if (res[0]!=0) res[0][60]=a29;
  a29=cos(a5);
  a18=cos(a1);
  a19=(a29*a18);
  a24=(a17*a26);
  a13=(a24*a109);
  a19=(a19-a13);
  a19=(a25*a19);
  a13=sin(a1);
  a10=(a29*a13);
  a15=(a24*a108);
  a10=(a10+a15);
  a15=(a39*a10);
  a19=(a19-a15);
  a10=(a6*a10);
  a19=(a19-a10);
  if (res[0]!=0) res[0][61]=a19;
  a19=sin(a5);
  a18=(a19*a18);
  a26=(a110*a26);
  a109=(a26*a109);
  a18=(a18+a109);
  a25=(a25*a18);
  a108=(a26*a108);
  a13=(a19*a13);
  a108=(a108-a13);
  a39=(a39*a108);
  a25=(a25+a39);
  a108=(a6*a108);
  a25=(a25+a108);
  if (res[0]!=0) res[0][62]=a25;
  a25=cos(a3);
  a108=sin(a1);
  a39=(a25*a108);
  a39=(a14*a39);
  a13=cos(a1);
  a18=(a25*a13);
  a109=(a47*a18);
  a39=(a39+a109);
  a18=(a6*a18);
  a39=(a39+a18);
  a39=(-a39);
  if (res[0]!=0) res[0][63]=a39;
  a39=cos(a5);
  a18=cos(a1);
  a109=(a39*a18);
  a10=(a34*a35);
  a15=(a10*a108);
  a109=(a109-a15);
  a109=(a14*a109);
  a15=sin(a1);
  a87=(a39*a15);
  a11=(a10*a13);
  a87=(a87+a11);
  a11=(a47*a87);
  a109=(a109-a11);
  a87=(a6*a87);
  a109=(a109-a87);
  if (res[0]!=0) res[0][64]=a109;
  a109=sin(a5);
  a18=(a109*a18);
  a35=(a22*a35);
  a108=(a35*a108);
  a18=(a18+a108);
  a14=(a14*a18);
  a13=(a35*a13);
  a15=(a109*a15);
  a13=(a13-a15);
  a47=(a47*a13);
  a14=(a14+a47);
  a13=(a6*a13);
  a14=(a14+a13);
  if (res[0]!=0) res[0][65]=a14;
  a14=cos(a3);
  a13=cos(a1);
  a47=(a14*a13);
  a15=(a79*a47);
  a15=(a75*a15);
  a18=sin(a1);
  a108=(a14*a18);
  a108=(a45*a108);
  a87=(a69*a47);
  a87=(a64*a87);
  a108=(a108+a87);
  a15=(a15-a108);
  a108=(a80*a47);
  a47=(a6*a47);
  a108=(a108+a47);
  a15=(a15-a108);
  if (res[0]!=0) res[0][66]=a15;
  a15=cos(a5);
  a108=cos(a1);
  a47=(a15*a108);
  a87=(a7*a55);
  a11=(a87*a18);
  a47=(a47-a11);
  a47=(a45*a47);
  a11=sin(a1);
  a89=(a15*a11);
  a9=(a87*a13);
  a89=(a89+a9);
  a9=(a69*a89);
  a9=(a64*a9);
  a47=(a47-a9);
  a9=(a79*a89);
  a9=(a75*a9);
  a47=(a47+a9);
  a9=(a80*a89);
  a89=(a6*a89);
  a9=(a9+a89);
  a47=(a47-a9);
  if (res[0]!=0) res[0][67]=a47;
  a47=sin(a5);
  a108=(a47*a108);
  a55=(a33*a55);
  a18=(a55*a18);
  a108=(a108+a18);
  a45=(a45*a108);
  a13=(a55*a13);
  a11=(a47*a11);
  a13=(a13-a11);
  a11=(a69*a13);
  a11=(a64*a11);
  a45=(a45+a11);
  a11=(a79*a13);
  a11=(a75*a11);
  a45=(a45-a11);
  a80=(a80*a13);
  a13=(a6*a13);
  a80=(a80+a13);
  a45=(a45+a80);
  if (res[0]!=0) res[0][68]=a45;
  a45=cos(a3);
  a80=cos(a1);
  a13=(a45*a80);
  a11=(a107*a13);
  a11=(a103*a11);
  a108=sin(a1);
  a18=(a45*a108);
  a18=(a81*a18);
  a9=(a97*a13);
  a9=(a92*a9);
  a18=(a18+a9);
  a11=(a11-a18);
  a18=(a106*a13);
  a13=(a6*a13);
  a18=(a18+a13);
  a11=(a11-a18);
  if (res[0]!=0) res[0][69]=a11;
  a11=cos(a5);
  a18=cos(a1);
  a13=(a11*a18);
  a9=(a23*a84);
  a89=(a9*a108);
  a13=(a13-a89);
  a13=(a81*a13);
  a1=sin(a1);
  a89=(a11*a1);
  a36=(a9*a80);
  a89=(a89+a36);
  a36=(a97*a89);
  a36=(a92*a36);
  a13=(a13-a36);
  a36=(a107*a89);
  a36=(a103*a36);
  a13=(a13+a36);
  a36=(a106*a89);
  a89=(a6*a89);
  a36=(a36+a89);
  a13=(a13-a36);
  if (res[0]!=0) res[0][70]=a13;
  a5=sin(a5);
  a18=(a5*a18);
  a84=(a111*a84);
  a108=(a84*a108);
  a18=(a18+a108);
  a81=(a81*a18);
  a80=(a84*a80);
  a1=(a5*a1);
  a80=(a80-a1);
  a1=(a97*a80);
  a1=(a92*a1);
  a81=(a81+a1);
  a1=(a107*a80);
  a1=(a103*a1);
  a81=(a81-a1);
  a106=(a106*a80);
  a6=(a6*a80);
  a106=(a106+a6);
  a81=(a81+a106);
  if (res[0]!=0) res[0][71]=a81;
  a8=(a8*a31);
  a81=sin(a28);
  a81=(a27*a81);
  a8=(a8*a81);
  a106=sin(a3);
  a6=cos(a28);
  a27=(a27*a6);
  a106=(a106*a27);
  a8=(a8+a106);
  if (res[0]!=0) res[0][72]=a8;
  a12=(a12*a30);
  a0=(a0*a31);
  a12=(a12-a0);
  a12=(a12*a81);
  a20=(a20*a32);
  a20=(a20*a27);
  a12=(a12+a20);
  a12=(-a12);
  if (res[0]!=0) res[0][73]=a12;
  a16=(a16*a32);
  a16=(a16*a27);
  a21=(a21*a31);
  a113=(a113*a30);
  a21=(a21+a113);
  a21=(a21*a81);
  a16=(a16-a21);
  if (res[0]!=0) res[0][74]=a16;
  a82=(a82*a41);
  a16=sin(a28);
  a16=(a38*a16);
  a82=(a82*a16);
  a21=sin(a3);
  a81=cos(a28);
  a38=(a38*a81);
  a21=(a21*a38);
  a82=(a82+a21);
  if (res[0]!=0) res[0][75]=a82;
  a29=(a29*a40);
  a24=(a24*a41);
  a29=(a29-a24);
  a29=(a29*a16);
  a17=(a17*a42);
  a17=(a17*a38);
  a29=(a29+a17);
  a29=(-a29);
  if (res[0]!=0) res[0][76]=a29;
  a110=(a110*a42);
  a110=(a110*a38);
  a26=(a26*a41);
  a19=(a19*a40);
  a26=(a26+a19);
  a26=(a26*a16);
  a110=(a110-a26);
  if (res[0]!=0) res[0][77]=a110;
  a25=(a25*a49);
  a110=sin(a28);
  a110=(a46*a110);
  a25=(a25*a110);
  a26=sin(a3);
  a16=cos(a28);
  a46=(a46*a16);
  a26=(a26*a46);
  a25=(a25+a26);
  if (res[0]!=0) res[0][78]=a25;
  a39=(a39*a48);
  a10=(a10*a49);
  a39=(a39-a10);
  a39=(a39*a110);
  a34=(a34*a50);
  a34=(a34*a46);
  a39=(a39+a34);
  a39=(-a39);
  if (res[0]!=0) res[0][79]=a39;
  a22=(a22*a50);
  a22=(a22*a46);
  a35=(a35*a49);
  a109=(a109*a48);
  a35=(a35+a109);
  a35=(a35*a110);
  a22=(a22-a35);
  if (res[0]!=0) res[0][80]=a22;
  a22=sin(a3);
  a35=cos(a28);
  a110=(a22*a35);
  a109=(a14*a71);
  a48=sin(a28);
  a49=(a109*a48);
  a110=(a110+a49);
  a110=(a64*a110);
  a49=cos(a28);
  a46=(a109*a49);
  a50=sin(a28);
  a39=(a22*a50);
  a46=(a46-a39);
  a46=(a75*a46);
  a110=(a110+a46);
  a46=(a78*a35);
  a39=(a22*a46);
  a34=(a78*a48);
  a10=(a109*a34);
  a39=(a39+a10);
  a110=(a110+a39);
  if (res[0]!=0) res[0][81]=a110;
  a110=(a15*a70);
  a39=(a87*a71);
  a110=(a110-a39);
  a39=(a110*a48);
  a7=(a7*a73);
  a10=(a7*a35);
  a39=(a39+a10);
  a39=(a64*a39);
  a10=(a110*a49);
  a25=(a7*a50);
  a10=(a10-a25);
  a10=(a75*a10);
  a39=(a39+a10);
  a10=(a110*a34);
  a25=(a7*a46);
  a10=(a10+a25);
  a39=(a39+a10);
  a39=(-a39);
  if (res[0]!=0) res[0][82]=a39;
  a33=(a33*a73);
  a35=(a33*a35);
  a71=(a55*a71);
  a70=(a47*a70);
  a71=(a71+a70);
  a48=(a71*a48);
  a35=(a35-a48);
  a64=(a64*a35);
  a50=(a33*a50);
  a49=(a71*a49);
  a50=(a50+a49);
  a75=(a75*a50);
  a64=(a64-a75);
  a46=(a33*a46);
  a34=(a71*a34);
  a46=(a46-a34);
  a64=(a64+a46);
  if (res[0]!=0) res[0][83]=a64;
  a3=sin(a3);
  a64=cos(a28);
  a46=(a3*a64);
  a34=(a45*a99);
  a75=sin(a28);
  a50=(a34*a75);
  a46=(a46+a50);
  a46=(a92*a46);
  a50=cos(a28);
  a49=(a34*a50);
  a28=sin(a28);
  a35=(a3*a28);
  a49=(a49-a35);
  a49=(a103*a49);
  a46=(a46+a49);
  a49=(a78*a64);
  a35=(a3*a49);
  a78=(a78*a75);
  a48=(a34*a78);
  a35=(a35+a48);
  a46=(a46+a35);
  if (res[0]!=0) res[0][84]=a46;
  a46=(a11*a98);
  a35=(a9*a99);
  a46=(a46-a35);
  a35=(a46*a75);
  a23=(a23*a101);
  a48=(a23*a64);
  a35=(a35+a48);
  a35=(a92*a35);
  a48=(a46*a50);
  a70=(a23*a28);
  a48=(a48-a70);
  a48=(a103*a48);
  a35=(a35+a48);
  a48=(a46*a78);
  a70=(a23*a49);
  a48=(a48+a70);
  a35=(a35+a48);
  a35=(-a35);
  if (res[0]!=0) res[0][85]=a35;
  a111=(a111*a101);
  a64=(a111*a64);
  a99=(a84*a99);
  a98=(a5*a98);
  a99=(a99+a98);
  a75=(a99*a75);
  a64=(a64-a75);
  a92=(a92*a64);
  a28=(a111*a28);
  a50=(a99*a50);
  a28=(a28+a50);
  a103=(a103*a28);
  a92=(a92-a103);
  a49=(a111*a49);
  a78=(a99*a78);
  a49=(a49-a78);
  a92=(a92+a49);
  if (res[0]!=0) res[0][86]=a92;
  a92=(a109*a79);
  a49=(a22*a77);
  a92=(a92+a49);
  a49=cos(a61);
  a49=(a63*a49);
  a78=sin(a61);
  a103=(a66*a78);
  a103=(a53*a103);
  a49=(a49-a103);
  a49=(a37*a49);
  a78=(a68*a78);
  a78=(a44*a78);
  a49=(a49-a78);
  a78=(a92*a49);
  a22=(a22*a72);
  a109=(a109*a69);
  a22=(a22-a109);
  a109=sin(a61);
  a63=(a63*a109);
  a109=cos(a61);
  a66=(a66*a109);
  a66=(a53*a66);
  a63=(a63+a66);
  a63=(a37*a63);
  a68=(a68*a109);
  a68=(a44*a68);
  a63=(a63+a68);
  a68=(a22*a63);
  a78=(a78-a68);
  if (res[0]!=0) res[0][87]=a78;
  a78=(a110*a69);
  a68=(a7*a72);
  a78=(a78-a68);
  a68=(a78*a63);
  a110=(a110*a79);
  a7=(a7*a77);
  a110=(a110+a7);
  a7=(a110*a49);
  a68=(a68+a7);
  a68=(-a68);
  if (res[0]!=0) res[0][88]=a68;
  a77=(a33*a77);
  a79=(a71*a79);
  a77=(a77-a79);
  a49=(a77*a49);
  a71=(a71*a69);
  a33=(a33*a72);
  a71=(a71+a33);
  a63=(a71*a63);
  a49=(a49-a63);
  if (res[0]!=0) res[0][89]=a49;
  a49=(a34*a107);
  a63=(a3*a105);
  a49=(a49+a63);
  a63=cos(a61);
  a63=(a91*a63);
  a33=sin(a61);
  a72=(a94*a33);
  a72=(a60*a72);
  a63=(a63-a72);
  a63=(a56*a63);
  a33=(a96*a33);
  a33=(a44*a33);
  a63=(a63-a33);
  a33=(a49*a63);
  a3=(a3*a100);
  a34=(a34*a97);
  a3=(a3-a34);
  a34=sin(a61);
  a91=(a91*a34);
  a61=cos(a61);
  a94=(a94*a61);
  a94=(a60*a94);
  a91=(a91+a94);
  a91=(a56*a91);
  a96=(a96*a61);
  a96=(a44*a96);
  a91=(a91+a96);
  a96=(a3*a91);
  a33=(a33-a96);
  if (res[0]!=0) res[0][90]=a33;
  a33=(a46*a97);
  a96=(a23*a100);
  a33=(a33-a96);
  a96=(a33*a91);
  a46=(a46*a107);
  a23=(a23*a105);
  a46=(a46+a23);
  a23=(a46*a63);
  a96=(a96+a23);
  a96=(-a96);
  if (res[0]!=0) res[0][91]=a96;
  a105=(a111*a105);
  a107=(a99*a107);
  a105=(a105-a107);
  a63=(a105*a63);
  a99=(a99*a97);
  a111=(a111*a100);
  a99=(a99+a111);
  a91=(a99*a91);
  a63=(a63-a91);
  if (res[0]!=0) res[0][92]=a63;
  a14=(a14*a54);
  a63=cos(a43);
  a63=(a44*a63);
  a91=sin(a43);
  a91=(a53*a91);
  a91=(a37*a91);
  a63=(a63+a91);
  a91=(a14*a63);
  a111=sin(a43);
  a100=(a65*a111);
  a100=(a44*a100);
  a97=cos(a43);
  a65=(a65*a97);
  a65=(a53*a65);
  a65=(a37*a65);
  a100=(a100-a65);
  a65=(a22*a100);
  a91=(a91+a65);
  a97=(a57*a97);
  a53=(a53*a97);
  a53=(a37*a53);
  a57=(a57*a111);
  a57=(a44*a57);
  a53=(a53-a57);
  a57=(a92*a53);
  a91=(a91+a57);
  if (res[0]!=0) res[0][93]=a91;
  a87=(a87*a54);
  a15=(a15*a59);
  a87=(a87+a15);
  a15=(a87*a63);
  a91=(a78*a100);
  a15=(a15+a91);
  a91=(a110*a53);
  a15=(a15-a91);
  if (res[0]!=0) res[0][94]=a15;
  a47=(a47*a59);
  a55=(a55*a54);
  a47=(a47-a55);
  a63=(a47*a63);
  a100=(a71*a100);
  a63=(a63+a100);
  a53=(a77*a53);
  a63=(a63+a53);
  if (res[0]!=0) res[0][95]=a63;
  a45=(a45*a83);
  a63=cos(a43);
  a63=(a44*a63);
  a53=sin(a43);
  a53=(a60*a53);
  a53=(a56*a53);
  a63=(a63+a53);
  a53=(a45*a63);
  a100=sin(a43);
  a55=(a93*a100);
  a55=(a44*a55);
  a43=cos(a43);
  a93=(a93*a43);
  a93=(a60*a93);
  a93=(a56*a93);
  a55=(a55-a93);
  a93=(a3*a55);
  a53=(a53+a93);
  a43=(a86*a43);
  a60=(a60*a43);
  a60=(a56*a60);
  a86=(a86*a100);
  a44=(a44*a86);
  a60=(a60-a44);
  a44=(a49*a60);
  a53=(a53+a44);
  if (res[0]!=0) res[0][96]=a53;
  a9=(a9*a83);
  a11=(a11*a88);
  a9=(a9+a11);
  a11=(a9*a63);
  a53=(a33*a55);
  a11=(a11+a53);
  a53=(a46*a60);
  a11=(a11-a53);
  if (res[0]!=0) res[0][97]=a11;
  a5=(a5*a88);
  a84=(a84*a83);
  a5=(a5-a84);
  a63=(a5*a63);
  a55=(a99*a55);
  a63=(a63+a55);
  a60=(a105*a60);
  a63=(a63+a60);
  if (res[0]!=0) res[0][98]=a63;
  a63=cos(a52);
  a74=(a74*a63);
  a60=sin(a52);
  a76=(a76*a60);
  a74=(a74-a76);
  a74=(a37*a74);
  a92=(a92*a74);
  a51=(a51*a63);
  a51=(a37*a51);
  a14=(a14*a51);
  a62=(a62*a60);
  a67=(a67*a63);
  a62=(a62+a67);
  a37=(a37*a62);
  a22=(a22*a37);
  a14=(a14+a22);
  a92=(a92-a14);
  if (res[0]!=0) res[0][99]=a92;
  a87=(a87*a51);
  a78=(a78*a37);
  a87=(a87+a78);
  a110=(a110*a74);
  a87=(a87+a110);
  a87=(-a87);
  if (res[0]!=0) res[0][100]=a87;
  a77=(a77*a74);
  a47=(a47*a51);
  a71=(a71*a37);
  a47=(a47+a71);
  a77=(a77-a47);
  if (res[0]!=0) res[0][101]=a77;
  a77=cos(a52);
  a102=(a102*a77);
  a52=sin(a52);
  a104=(a104*a52);
  a102=(a102-a104);
  a102=(a56*a102);
  a49=(a49*a102);
  a58=(a58*a77);
  a58=(a56*a58);
  a45=(a45*a58);
  a90=(a90*a52);
  a95=(a95*a77);
  a90=(a90+a95);
  a56=(a56*a90);
  a3=(a3*a56);
  a45=(a45+a3);
  a49=(a49-a45);
  if (res[0]!=0) res[0][102]=a49;
  a9=(a9*a58);
  a33=(a33*a56);
  a9=(a9+a33);
  a46=(a46*a102);
  a9=(a9+a46);
  a9=(-a9);
  if (res[0]!=0) res[0][103]=a9;
  a105=(a105*a102);
  a5=(a5*a58);
  a99=(a99*a56);
  a5=(a5+a99);
  a105=(a105-a5);
  if (res[0]!=0) res[0][104]=a105;
  return 0;
}

CASADI_SYMBOL_EXPORT int f_J(const casadi_real** arg, casadi_real** res, casadi_int* iw, casadi_real* w, int mem){
  return casadi_f0(arg, res, iw, w, mem);
}

CASADI_SYMBOL_EXPORT int f_J_alloc_mem(void) {
  return 0;
}

CASADI_SYMBOL_EXPORT int f_J_init_mem(int mem) {
  return 0;
}

CASADI_SYMBOL_EXPORT void f_J_free_mem(int mem) {
}

CASADI_SYMBOL_EXPORT int f_J_checkout(void) {
  return 0;
}

CASADI_SYMBOL_EXPORT void f_J_release(int mem) {
}

CASADI_SYMBOL_EXPORT void f_J_incref(void) {
}

CASADI_SYMBOL_EXPORT void f_J_decref(void) {
}

CASADI_SYMBOL_EXPORT casadi_int f_J_n_in(void) { return 1;}

CASADI_SYMBOL_EXPORT casadi_int f_J_n_out(void) { return 1;}

CASADI_SYMBOL_EXPORT casadi_real f_J_default_in(casadi_int i){
  switch (i) {
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const char* f_J_name_in(casadi_int i){
  switch (i) {
    case 0: return "i0";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const char* f_J_name_out(casadi_int i){
  switch (i) {
    case 0: return "o0";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const casadi_int* f_J_sparsity_in(casadi_int i) {
  switch (i) {
    case 0: return casadi_s0;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const casadi_int* f_J_sparsity_out(casadi_int i) {
  switch (i) {
    case 0: return casadi_s1;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT int f_J_work(casadi_int *sz_arg, casadi_int* sz_res, casadi_int *sz_iw, casadi_int *sz_w) {
  if (sz_arg) *sz_arg = 1;
  if (sz_res) *sz_res = 1;
  if (sz_iw) *sz_iw = 0;
  if (sz_w) *sz_w = 0;
  return 0;
}

#ifdef MATLAB_MEX_FILE
void mex_f_J(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {
  casadi_int i;
  casadi_real w[227];
  casadi_int *iw = 0;
  const casadi_real* arg[1] = {0};
  casadi_real* res[1] = {0};
  if (argc>1) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"f_J\" failed. Too many input arguments (%d, max 1)", argc);
  if (resc>1) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"f_J\" failed. Too many output arguments (%d, max 1)", resc);
  if (--argc>=0) arg[0] = casadi_from_mex(argv[0], w, casadi_s0, w+112);
  --resc;
  res[0] = w+7;
  i = f_J(arg, res, iw, w+112, 0);
  if (i) mexErrMsgIdAndTxt("Casadi:RuntimeError","Evaluation of \"f_J\" failed.");
  if (res[0]) resv[0] = casadi_to_mex(casadi_s1, res[0]);
}
#endif

casadi_int main_f_J(casadi_int argc, char* argv[]) {
  casadi_int j;
  casadi_real* a;
  const casadi_real* r;
  casadi_int flag;
  casadi_int *iw = 0;
  casadi_real w[227];
  const casadi_real* arg[1];
  casadi_real* res[1];
  arg[0] = w+0;
  res[0] = w+7;
  a = w;
  for (j=0; j<7; ++j) if (scanf("%lg", a++)<=0) return 2;
  flag = f_J(arg, res, iw, w+112, 0);
  if (flag) return flag;
  r = w+7;
  for (j=0; j<105; ++j) CASADI_PRINTF("%g ", *r++);
  CASADI_PRINTF("\n");
  return 0;
}


#ifdef MATLAB_MEX_FILE
void mexFunction(int resc, mxArray *resv[], int argc, const mxArray *argv[]) {
  char buf[4];
  int buf_ok = argc > 0 && !mxGetString(*argv, buf, sizeof(buf));
  if (!buf_ok) {
    mex_f_J(resc, resv, argc, argv);
    return;
  } else if (strcmp(buf, "f_J")==0) {
    mex_f_J(resc, resv, argc-1, argv+1);
    return;
  }
  mexErrMsgTxt("First input should be a command string. Possible values: 'f_J'");
}
#endif
int main(int argc, char* argv[]) {
  if (argc<2) {
    /* name error */
  } else if (strcmp(argv[1], "f_J")==0) {
    return main_f_J(argc-2, argv+2);
  }
  fprintf(stderr, "First input should be a command string. Possible values: 'f_J'\nNote: you may use function.generate_input to create a command string.\n");
  return 1;
}
#ifdef __cplusplus
} /* extern "C" */
#endif
