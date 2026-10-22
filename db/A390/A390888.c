/*
 * A390888  a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * Stirling2(n,k).
 * https://oeis.org/A390888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache   = NULL;   /* computed a(0 .. lenA-1) */
static seq_t *pow3_cache = NULL;  /* 3^i for i = 0 .. lenPow3-1 */
static size_t a_cap = 0, pow3_cap = 0;
static idx_t  a_len = 0, pow3_len = 0;

/* ensure pow3_cache contains values up to index n (inclusive) */
static void ensure_pow3_up_to(idx_t n)
{
    if (pow3_len > n) return;

    if (!pow3_cache) {
        pow3_cap = 8;
        pow3_cache = (seq_t*)malloc(pow3_cap * sizeof(seq_t));
    }
    while (pow3_len <= n) {
        if (pow3_len == 0)
            pow3_cache[0] = 1;
        else
            pow3_cache[pow3_len] = pow3_cache[pow3_len-1] * (seq_t)3;

        ++pow3_len;
        if (pow3_len == pow3_cap) {
            pow3_cap *= 2;
            pow3_cache = (seq_t*)realloc(pow3_cache, pow3_cap * sizeof(seq_t));
        }
    }
}

/* compute a(0..n) using the recurrence:
   a(0)=1; a(m)=2*sum_{j=0}^{m-1} binom(m-1,j)*3^{m-1-j}*a(j)   */
static void compute_a_up_to(idx_t n)
{
    if (a_len > n) return;

    if (!a_cache) {
        a_cap = 8;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
    }

    ensure_pow3_up_to(n);   /* we need powers up to n-1 */

    while (a_len <= n) {
        if (a_len == 0) {
            a_cache[0] = 1;
        } else {
            seq_t sum = 0;
            seq_t binom = 1;            /* binom(m-1,0) */
            idx_t parent = a_len - 1;   /* m-1 */

            for (idx_t j = 0; j <= parent; ++j) {
                /* term = binom * 3^(parent-j) * a(j) */
                seq_t term = binom * pow3_cache[parent - j];
                term *= a_cache[j];
                sum += term;

                /* update binom to binom(parent, j+1) */
                if (j < parent) {
                    binom = (binom * (seq_t)(parent - j)) / (idx_t)(j + 1);
                }
            }
            a_cache[a_len] = sum * (seq_t)2;
        }

        ++a_len;
        if (a_len == a_cap) {
            a_cap *= 2;
            a_cache = (seq_t*)realloc(a_cache, a_cap * sizeof(seq_t));
        }
    }
}

seq_t a(idx_t n)
{
    compute_a_up_to(n);
    return a_cache[n];
}
