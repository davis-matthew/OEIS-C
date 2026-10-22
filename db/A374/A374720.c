/*
 * A374720  Permutation rank of the initial state S of length n in an RC4-like
 *          Key Scheduling Algorithm with key comprising numbers 1 to n.
 * https://oeis.org/A374720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the lexicographic rank of the permutation obtained
 * by running the RC4-like Key Scheduling Algorithm on the key
 * [1,2,…,n].  The rank is computed using the factorial number system.  */
#include <stdlib.h>
#include <stdint.h>

/* Fenwick tree helpers */
static inline void bit_add(idx_t *bit, idx_t n, idx_t idx, idx_t val) {
    /* indices in bit are 1‑based */
    for (idx += 1; idx <= n; idx += idx & -idx)
        bit[idx] += val;
}
static inline idx_t bit_sum(idx_t *bit, idx_t idx) {
    idx_t s = 0;
    for (idx += 1; idx > 0; idx -= idx & -idx)
        s += bit[idx];
    return s;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* not defined but safe */

    /* Generate the permutation via the RC4‑like KSA */
    idx_t *S = (idx_t*)malloc(n * sizeof(idx_t));
    if (!S) return (seq_t)0;        /* out of memory – error handling omitted */

    for (idx_t i = 0; i < n; ++i)
        S[i] = i;
    idx_t j = 0;
    for (idx_t i = 0; i < n; ++i) {
        j = (j + S[i] + (i + 1)) % n;
        /* swap S[i] and S[j] */
        idx_t t = S[i];
        S[i] = S[j];
        S[j] = t;
    }

    /* Precompute factorials 0! … n! in seq_t */
    seq_t *fact = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!fact) { free(S); return (seq_t)0; }
    fact[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        fact[k] = fact[k - 1] * (seq_t)k;

    /* Build a Fenwick tree for the unused numbers (initially all present) */
    idx_t *bit = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!bit) { free(S); free(fact); return (seq_t)0; }
    for (idx_t i = 0; i <= n; ++i)
        bit[i] = 0;
    for (idx_t i = 0; i < n; ++i)
        bit_add(bit, n, i, 1);  /* mark all indices as unused */

    /* Compute rank using the factorial number system */
    seq_t rank = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        idx_t x = S[i];
        idx_t cnt = (x == 0) ? 0 : bit_sum(bit, x - 1);
        rank += (seq_t)cnt * fact[n - 1 - i];
        bit_add(bit, n, x, (idx_t)-1);  /* remove x from the set */
    }

    /* Clean up */
    free(S);
    free(fact);
    free(bit);

    return rank;
}
