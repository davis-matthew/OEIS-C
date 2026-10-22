/*
 * A356336  Expansion of e.g.f. ( Product_{k>0} 1/(1-x^k)^(1/k) )^(1/(1-x)).
 * https://oeis.org/A356336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=1..n} A356297(k) * binomial(n-1,k-1) * a(n-k),
 * with a(0) = 1.
 *
 * A356297(k) = k! * Σ_{j=1..k} τ(j)/j  (τ(j) = number of divisors of j),
 * which simplifies to the recurrence
 *    B[1] = 1,
 *    B[k] = k * B[k-1] + (k-1)! * τ(k)    for k ≥ 2.
 * The recurrence for a(n) uses these integer values B[k].
 *
 * The code is fully static, thread‑safe for independent calls, uses only
 * integer arithmetic, and works for any seq_t / idx_t provided by the
 * build.  No global state is assumed beyond the static caches.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static caches */
    static seq_t *A = NULL;          /* a[0..] */
    static size_t Alen = 0;          /* current allocated size for A */
    static seq_t *B = NULL;          /* B[k] = A356297(k) , k>=1, B[0] unused */
    static size_t Blen = 0;          /* current allocated size for B */
    static idx_t *D = NULL;          /* divisor counts τ(k) */
    static size_t Dlen = 0;          /* current allocated size for D */

    /* required size (0..n inclusive) */
    size_t need = (size_t)n + 1;

    /* --- extend divisor counts up to n --- */
    if (Dlen <= n) {
        size_t oldD = Dlen;
        size_t newD = need;
        D = (idx_t *)realloc(D, newD * sizeof(idx_t));
        for (size_t i = oldD; i < newD; ++i) D[i] = 0;
        /* simple sieve to count divisors */
        for (size_t i = 1; i < newD; ++i) {
            for (size_t j = i; j < newD; j += i) {
                ++D[j];
            }
        }
        Dlen = newD;
    }

    /* --- extend B array (A356297) up to n --- */
    if (Blen <= n) {
        size_t oldB = Blen;
        size_t newB = need;
        B = (seq_t *)realloc(B, newB * sizeof(seq_t));
        /* B[0] unused, set to 0 for recurrence */
        if (oldB == 0) {
            B[0] = 0;
            oldB = 1;  /* treat B[0] as existing */
        }
        /* keep factorial of previous k ( (oldB-1)! ) */
        static seq_t fac = 1;      /* factorial of (k-1) ! */
        idx_t prev_k = (idx_t)(oldB - 1);   /* last computed k */

        /* compute B from prev_k+1 up to n */
        for (idx_t k = prev_k + 1; k <= n; ++k) {
            /* B[k] = k * B[k-1] + (k-1)! * τ(k) */
            seq_t term = (seq_t)k * B[k - 1] + fac * (seq_t)D[k];
            B[k] = term;
            /* update factorial: fac becomes k! */
            fac = fac * (seq_t)k;
        }
        Blen = newB;
    }

    /* --- extend A array up to n --- */
    if (Alen <= n) {
        size_t oldA = Alen;
        size_t newA = need;
        A = (seq_t *)realloc(A, newA * sizeof(seq_t));
        /* set a[0] = 1 if this is first allocation */
        if (oldA == 0) {
            A[0] = (seq_t)1;
            oldA = 1;
        }
        /* compute a[oldA] .. a[n] */
        for (idx_t i = oldA; i <= n; ++i) {
            seq_t sum = 0;
            seq_t binom = 1;   /* binomial (i-1 choose 0) */
            for (idx_t k = 1; k <= i; ++k) {
                sum += B[k] * binom * A[i - k];
                if (k < i) {
                    /* update binom to C(i-1, k) from C(i-1, k-1) */
                    binom = (binom * ((seq_t)(i - k))) / (seq_t)k;
                }
            }
            A[i] = sum;
        }
        Alen = newA;
    }

    return A[(size_t)n];
}
