/*
 * A263923  Expansion of psi(-x^3)^2 * f(-x^2)^3 / f(-x)^2 in powers of x
 *          where psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A263923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A263923 – expansion of ψ(−x³)² · f(−x²)³ / f(−x)² in powers of x */

/* build‑specific integer types seq_t (value) and idx_t (index) are supplied */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* period for the exponents –s_k of the product ∏(1−q^k)^(-s_k) */
static const int period[12] = {
   -2,  /* s_1 =  2  → f = -s_1 = -2  : divide by (1−q)^2      */
    1,  /* s_2 = -1  → f =  1  : multiply by (1−q^2)        */
    0,  /* s_3 =  0  → f =  0  : skip                     */
    1,  /* s_4 = -1  → f =  1  : multiply by (1−q^4)        */
   -2,  /* s_5 =  2  → f = -2  : divide by (1−q^5)²        */
    1,  /* s_6 = -1  → f =  1  : multiply by (1−q^6)        */
   -2,  /* s_7 =  2  → f = -2  : divide by (1−q^7)²        */
    1,  /* s_8 = -1  → f =  1  : multiply by (1−q^8)        */
    0,  /* s_9 =  0  → f =  0  : skip                     */
    1,  /* s_10= -1  → f =  1  : multiply by (1−q^10)       */
   -2,  /* s_11=  2  → f = -2  : divide by (1−q^11)²       */
    3   /* s_12= -3  → f =  3  : multiply by (1−q^12)³      */
};

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *coeff;
#ifdef __cplusplus
    coeff = new seq_t[N+1]();          /* value‑initialised */
#else
    coeff = (seq_t*)calloc(N+1, sizeof(seq_t));
#endif
    if (!coeff) return 0;
    coeff[0] = (seq_t)1;

    for (size_t k = 1; k <= N; ++k) {
        int f = period[(k-1) % 12];
        if (f == 0) continue;

        if (f > 0) {                   /* multiply by (1−q^k)^f */
            for (int rep = 0; rep < f; ++rep) {
                for (size_t i = N; i >= k; --i) {
                    coeff[i] -= coeff[i - k];
                }
            }
        } else {                       /* divide by (1−q^k)^(-f) */
            int m = -f;                /* m = 2 for all negatives in this sequence */
            for (int rep = 0; rep < m; ++rep) {
                for (size_t i = k; i <= N; ++i) {
                    coeff[i] += coeff[i - k];
                }
            }
        }
    }

    seq_t result = coeff[N];
#ifdef __cplusplus
    delete [] coeff;
#else
    free(coeff);
#endif
    return result;
}
