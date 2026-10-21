/*
 * A267508  Smallest number "c-equivalent" to n.
 * https://oeis.org/A267508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A267508: for a given integer n (index ≥ 1) find the
   smallest integer that is c‑equivalent to n.
   The algorithm:
   1.  Divide the binary representation of n into substrings
       that start at each bit‑1 and end just before the next bit‑1.
   2.  Order these substrings to minimise the resulting binary number
       (the standard “minimal concatenation” order: a+b < b+a).
   3.  Concatenate the ordered substrings and return the value. */

#include <stdlib.h>
#include <stdint.h>

/* Helper structure for a substring: its value and bit length. */
struct seg { seq_t val; unsigned len; };

/* Comparator for qsort: order strings by the minimal resulting concatenation. */
static int seg_cmp(const void *a, const void *b)
{
    const struct seg *A = (const struct seg *)a;
    const struct seg *B = (const struct seg *)b;
    seq_t ab = (A->val << B->len) | B->val;
    seq_t ba = (B->val << A->len) | A->val;
    if (ab < ba) return -1;
    if (ab > ba) return 1;
    return 0;
}

/* Main function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety, though n≥1 in OEIS */

    /* Step 1: collect positions of 1‑bits (most significant → least). */
    unsigned pos[64];
    unsigned k = 0;
    for (int i = 63; i >= 0; --i) {
        if (n & ((idx_t)1ULL << i)) pos[k++] = i;
    }

    /* Step 2: build the substrings. */
    struct seg s[64];
    for (unsigned i = 0; i < k; ++i) {
        unsigned next = (i + 1 < k) ? pos[i + 1] : 0;
        unsigned shift = (i + 1 < k) ? next + 1 : 0;
        unsigned len   = (i + 1 < k) ? (pos[i] - next) : (pos[i] + 1);

        seq_t mask;
        if (len >= (sizeof(seq_t) * 8)) {
            mask = ~(seq_t)0;                     /* all ones for full width */
        } else {
            mask = ((seq_t)1 << len) - 1;
        }

        seq_t val = ((seq_t)(n >> shift)) & mask;
        s[i].val = val;
        s[i].len = len;
    }

    /* Step 3: sort the substrings to obtain the minimal concatenation. */
    qsort(s, k, sizeof(s[0]), seg_cmp);

    /* Concatenate the sorted substrings. */
    seq_t res = 0;
    for (unsigned i = 0; i < k; ++i) {
        res = (res << s[i].len) | s[i].val;
    }
    return res;
}
