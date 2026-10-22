/*
 * A318724  Let f(0) = 0 and f(t*4^k + u) = i^t * ((1+i) * 2^k - f(u)) for any
 *          t in {1, 2, 3} and k >= 0 and u such that 0 <= u < 4^k (i denoting
 *          the imaginary unit); for any n >= 0, let g(n) = (f(A042968(n)) - 1
 *          - i) / 2; a(n) is the square of the modulus of g(n).
 * https://oeis.org/A318724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 222, u16 n <= 9122, u32 n <= 9122, u64 n <= 9122,
 *            u128 n <= 9122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements OEIS A318724:
   a(n) = |((f(A042968(n)) - 1 - i) / 2)|^2
   where f is defined recursively by
     f(0) = 0
     f(t·4^k + u) = i^t·((1+i)·2^k - f(u)),  t∈{1,2,3}, k≥0, 0≤u<4^k
   A042968(n) is the nth positive integer not divisible by 4,
   i.e. m = n + 1 + n/3 for n ≥ 0.
   The function uses 64‑bit signed arithmetic for the intermediate real
   and imaginary parts; the final result fits into seq_t.
*/

typedef struct { long long r, i; } cplx_t;

static inline cplx_t f_rec(idx_t m) {
    if (m == 0)
        return (cplx_t){0, 0};

    /* Determine the highest power of 4 ≤ m:  
       4^k = 1 << (2*k),   k = floor(log2(m)/2)  */
    int k = (63 - __builtin_clzll(m)) >> 1;
    idx_t pow4k = ((idx_t)1) << (2 * k);

    idx_t t = m >> (2 * k);            /* the high base‑4 digit (1…3) */
    idx_t u = m & (pow4k - 1);         /* the lower part */

    cplx_t fu = f_rec(u);              /* f(u) */
    long long two_k = 1LL << k;         /* 2^k */

    /* (1+i)·2^k - f(u) */
    long long rr = two_k - fu.r;
    long long ii = two_k - fu.i;

    /* multiply by i^t */
    long long r, i;
    switch (t) {
        case 0: r = rr; i = ii; break;
        case 1: r = -ii; i = rr; break;
        case 2: r = -rr; i = -ii; break;
        case 3: r = ii;  i = -rr; break;
        default: r = rr; i = ii;   /* should never happen */
    }
    return (cplx_t){r, i};
}

seq_t a(idx_t n) {
    /* A042968(n) = n + 1 + n/3  (numbers not divisible by 4) */
    idx_t m = n + 1 + n / 3;

    cplx_t f = f_rec(m);

    long long dr = f.r - 1;   /* real part of f - (1+i) */
    long long di = f.i - 1;   /* imag part of f - (1+i) */

    long long sumsq = dr * dr + di * di;   /* |f - (1+i)|^2 */
    seq_t res = (seq_t)(sumsq / 4);        /* divide by 4: modulus squared of g */
    return res;
}
