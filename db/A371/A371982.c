/*
 * A371982  Numbers k which are the sum k = b+c where the digits of {k, b ,c}
 *          together give 0,1,...,9 exactly once.
 * https://oeis.org/A371982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, nonn, base, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A371982: Numbers k which are the sum k = b + c where the digits of 
   {k, b ,c} together give 0,1,...,9 exactly once. 
   Computed by exhaustive enumeration of all 10‑digit permutations 
   and splitting them into k, b, c. 
*/
#include <stdlib.h>
#include <stdint.h>

/* Helper: reverse a sub‑array perm[l .. r] (inclusive) */
static void reverse_bits(int *perm, int l, int r)
{
    while (l < r) {
        int tmp = perm[l];
        perm[l] = perm[r];
        perm[r] = tmp;
        ++l;
        --r;
    }
}

/* Helper: generate the next lexicographic permutation of perm[10].
   Returns 0 when no further permutation exists. */
static int next_permutation(int *perm)
{
    int i = 8;
    while (i >= 0 && perm[i] > perm[i + 1]) i--;
    if (i < 0) return 0;
    int j = 9;
    while (perm[j] < perm[i]) j--;
    int tmp = perm[i];
    perm[i] = perm[j];
    perm[j] = tmp;
    reverse_bits(perm, i + 1, 9);
    return 1;
}

/* Comparison function for qsort on seq_t */
static int seq_cmp(const void *A, const void *B)
{
    const seq_t *a = (const seq_t *)A;
    const seq_t *b = (const seq_t *)B;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static storage for the generated sequence */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    if (terms == NULL) {
        /* Pre‑computation of all terms */
        int perm[10];
        for (int i = 0; i < 10; ++i) perm[i] = i;

        /* Pre‑compute powers of 10 up to 10^10 */
        uint64_t pow10[11];
        pow10[0] = 1;
        for (int i = 1; i <= 10; ++i) pow10[i] = pow10[i - 1] * 10ULL;

        /* Temporary arrays for prefix sums and candidate pushing */
        uint64_t prefix[11];
        do {
            /* k must start with a non‑zero digit */
            if (perm[0] == 0) continue;

            /* Build prefix sums of the current permutation */
            prefix[0] = 0;
            for (int d = 0; d < 10; ++d)
                prefix[d + 1] = prefix[d] * 10ULL + (uint64_t)perm[d];

            /* Try all split positions i (k), j (b) */
            for (int i = 1; i <= 8; ++i) {
                if (perm[i] == 0) continue;            /* b cannot start with 0 */
                uint64_t k = prefix[i];
                for (int j = i + 1; j <= 9; ++j) {
                    if (perm[j] == 0) continue;        /* c cannot start with 0 */
                    uint64_t b = prefix[j] - k * pow10[j - i];
                    uint64_t c = prefix[10] - prefix[j] * pow10[10 - j];
                    if (b + c == k) {
                        /* store k if not already reserved */
                        if (terms_len == terms_cap) {
                            terms_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
                            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                        }
                        terms[terms_len++] = (seq_t)k;
                    }
                }
            }
        } while (next_permutation(perm));

        /* Sort and deduplicate the collected terms */
        if (terms_len > 1) {
            qsort(terms, terms_len, sizeof(seq_t), seq_cmp);
            size_t uniq = 1;
            for (size_t i = 1; i < terms_len; ++i) {
                if (terms[i] != terms[uniq - 1]) {
                    terms[uniq++] = terms[i];
                }
            }
            terms_len = uniq;
        }
    }

    /* Return the requested term; if n is out of bounds return 0 */
    if (n == 0 || (uint64_t)(n - 1) >= terms_len) return (seq_t)0;
    return terms[n - 1];
}
