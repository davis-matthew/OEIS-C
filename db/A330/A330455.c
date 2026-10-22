/*
 * A330455  Number of sets of nonempty multisets of nonempty sets of positive
 *          integers with total sum n.
 * https://oeis.org/A330455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A330455: Number of sets of nonempty multisets of nonempty sets of positive integers with total sum n.
   The generating function is 
       G(x) = product_{i>0} (1 + x^i)^{s_i}
   where s_i is the number of multisets of nonempty sets of positive integers with total sum i.
   s_i itself is given by the generating function  
       S(x) = product_{S} 1/(1 - x^{sum(S)})
   where the product is over all nonempty sets S of positive integers. This is equivalent to  
       S(x) = product_{w>=1} (1 - x^w)^{-p_d(w)}
   with p_d(w) the number of partitions of w into distinct parts.
   The coefficients are computed by the recurrences:
       b[0] = 1,
       b[n] = (1/n) * sum_{k=1..n} A_k * b[n-k]                (b[n] = s_n)
       where A_k = sum_{d|k} d * p_d(d)
       H[n] = sum_{d|n} n/d parity term: H[n] = sum_{d|n} ( (-1)^{n/d+1} * d * s_d )
       a[0] = 1,
       a[n] = (1/n) * sum_{k=1..n} H_k * a[n-k]
   The above formulas use only integer arithmetic and produce the exact integer coefficients.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static seq_t *alloc_seq_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n]();
#else
    return (seq_t*)calloc(n, sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* Work in 0‑based indices. */
    idx_t N = n;                       /* the requested index */
    size_t size = (size_t)(N + 1);     /* array size */

    /* Allocate temporary arrays */
    seq_t *mdist = alloc_seq_array(size);   /* partitions into distinct parts p_d(i) */
    seq_t *Ak    = alloc_seq_array(size);   /* A_k = sum_{d|k} d * p_d(d)   */
    seq_t *sb    = alloc_seq_array(size);   /* s_b = b_i the number s_i   */
    seq_t *Hk    = alloc_seq_array(size);   /* H_k as defined above       */
    seq_t *an    = alloc_seq_array(size);   /* result a(n) values         */

    /* ----- 1. Distinct partition numbers mdist[i] ----- */
    mdist[0] = 1;
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = N; j >= i; --j) {
            mdist[j] += mdist[j - i];
        }
    }

    /* ----- 2. Compute A_k ----- */
    memset(Ak, 0, size * sizeof(seq_t));
    for (idx_t d = 1; d <= N; ++d) {
        seq_t md = mdist[d];
        if (md == 0) continue;
        seq_t dseq = (seq_t)d;
        for (idx_t k = d; k <= N; k += d) {
            Ak[k] += dseq * md;
        }
    }

    /* ----- 3. Compute sb[n] = b[n] ----- */
    sb[0] = 1;
    for (idx_t nn = 1; nn <= N; ++nn) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= nn; ++k) {
            sum += Ak[k] * sb[nn - k];
        }
        sb[nn] = sum / (seq_t)(nn);  /* division is exact */
    }

    /* ----- 4. Compute H_k ----- */
    memset(Hk, 0, size * sizeof(seq_t));
    for (idx_t d = 1; d <= N; ++d) {
        seq_t sd = sb[d];
        if (sd == 0) continue;
        seq_t dseq = (seq_t)d;
        seq_t term = sd * dseq;           /* d * s_d */
        for (idx_t k = d; k <= N; k += d) {
            idx_t t = k / d;              /* t = k/d */
            if (t % 2 == 0) {             /* even: minus */
                Hk[k] -= term;
            } else {                       /* odd:  plus */
                Hk[k] += term;
            }
        }
    }

    /* ----- 5. Compute final a[n] ----- */
    an[0] = 1;
    for (idx_t nn = 1; nn <= N; ++nn) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= nn; ++k) {
            sum += Hk[k] * an[nn - k];
        }
        an[nn] = sum / (seq_t)(nn);   /* division is exact */
    }

    seq_t result = an[N];

#ifdef __cplusplus
    delete[] mdist;
    delete[] Ak;
    delete[] sb;
    delete[] Hk;
    delete[] an;
#else
    free(mdist);
    free(Ak);
    free(sb);
    free(Hk);
    free(an);
#endif

    return result;
}
