/*
 * A260255  Numbers that can be written as the sum of two nonnegative
 *          palindromes in base 10.
 * https://oeis.org/A260255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A260255 – numbers representable as the sum of two non‑negative
   palindromes in base 10.
   The implementation builds the necessary set of attainable numbers by
   generating all palindromes up to a limit and marking all pairwise sums.
   The sequence is memoised in a static array and extended on demand.
   The algorithm is O(L²) where L is the largest number considered; for
   n up to 10 000 the running time stays well below 2 s.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_palindrome.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* ---------- static state -------------------------------------------------- */
static seq_t *seq_array  = NULL;   /* array of reachable numbers             */
static size_t seq_len    = 0;      /* number of stored values                */
static idx_t  seq_limit  = 0;      /* largest number considered so far      */

/* ---------- bit‑set helpers ------------------------------------------------*/
static inline void set_bit(unsigned char *arr, idx_t pos)
{
    arr[pos >> 3] |= 1u << (pos & 7);
}

static inline bool get_bit(const unsigned char *arr, idx_t pos)
{
    return (arr[pos >> 3] >> (pos & 7)) & 1u;
}

/* ---------- recompute everything up to |limit| ----------------------------*/
static void recompute(idx_t limit)
{
    /* 1. collect all palindromic numbers ≤ limit */
    idx_t *pal = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    size_t pal_cnt = 0;

    for (idx_t i = 0; i <= limit; ++i)
        if (is_palindrome((seq_t)i, 10))
            pal[pal_cnt++] = i;

    /* 2. mark all sums of two palindromes ≤ limit */
    size_t reach_bytes = (limit + 8) >> 3;
    unsigned char *reach = (unsigned char*)calloc(reach_bytes, 1);

    for (size_t a = 0; a < pal_cnt; ++a) {
        for (size_t b = a; b < pal_cnt; ++b) {
            idx_t sum = pal[a] + pal[b];
            if (sum <= limit)
                set_bit(reach, sum);
        }
    }

    /* 3. build the new sequence from the reached numbers */
    seq_t *new_seq = seq_alloc_new_array(limit + 1);
    size_t new_len = 0;

    for (idx_t v = 0; v <= limit; ++v)
        if (get_bit(reach, v))
            new_seq[new_len++] = (seq_t)v;

    /* 4. free old data and replace */
    if (seq_array) {
        seq_free_array(seq_array);
    }
    free(pal);
    free(reach);

    seq_array = new_seq;
    seq_len   = new_len;
    seq_limit = limit;
}

/* ---------- required public function -------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, harmless */

    /* ensure at least n terms are available */
    if (seq_len < (size_t)n) {
        idx_t limit = (seq_limit) ? seq_limit : 1000;
        if (limit < n) limit = n * 10 + 100;  /* quick guarantee */

        while (seq_len < (size_t)n) {
            recompute(limit);
            if (seq_len >= (size_t)n) break;
            limit <<= 1;                /* double the search bound */
        }
    }

    /* sequence indices start at 1, array at 0 */
    return seq_array[n - 1];
}
