/*
 * A379250  a(1)=1; thereafter, a(n) is the number of coincidences between the
 *          sequence thus far and its terms rearranged in descending order.
 * https://oeis.org/A379250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; thereafter a(n) counts for n>1 the positions i (1≤i≤n-1)
 *            where a(i) equals the i‑th term of the multiset {a(1),…,a(n-1)}
 *            sorted in descending order
 * Range:     u8 n <= 2919, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- headers ------------------------------------------------------- */
#include <stdlib.h>        /* malloc, realloc, calloc, free          */
#include <stdint.h>        /* uint64_t types                         */
#include <stddef.h>        /* size_t                                 */

/* --- memoisation structures ------------------------------------- */
static seq_t *seq_arr    = NULL;  /* sequence values (a(1)→seq_arr[0])   */
static idx_t  seq_len    = 0;      /* number of values stored              */
static idx_t  seq_cap    = 0;      /* allocated capacity of seq_arr        */

static idx_t *cnt_arr    = NULL;  /* frequency of each value seen         */
static idx_t  cnt_cap    = 0;      /* allocated capacity of cnt_arr        */
static idx_t  max_val    = 0;      /* largest value seen so far             */

/* --- helpers ----------------------------------------------------- */
static void ensure_seq_capacity(idx_t need)
{
    if (seq_cap < need) {
        size_t new_cap = (seq_cap ? seq_cap << 1 : 32);
        while (new_cap < (size_t)need) new_cap <<= 1;
        if (seq_arr)
            seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        else
            seq_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_cap = (idx_t)new_cap;
    }
}

static void ensure_cnt_capacity(idx_t need)
{
    if (cnt_cap <= need) {
        size_t new_cap = (cnt_cap ? cnt_cap << 1 : 2);
        while (new_cap <= (size_t)need) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(cnt_arr, new_cap * sizeof(idx_t));
        for (idx_t i = cnt_cap; i < (idx_t)new_cap; ++i) tmp[i] = 0;
        cnt_arr = tmp;
        cnt_cap = (idx_t)new_cap;
    }
}

/* --- main sequence routine --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined offset            */

    /* initialise storage on first call */
    if (seq_cap == 0) {
        seq_arr = (seq_t *)malloc(32 * sizeof(seq_t));
        seq_cap = 0;
    }
    if (cnt_cap == 0) {
        cnt_arr = (idx_t *)calloc(2, sizeof(idx_t));
        cnt_cap = 2;
    }

    /* compute values up to n (if not already done) */
    while (seq_len < n) {
        idx_t cur = seq_len + 1;            /* 1‑based index of new term   */

        /* base case a(1) */
        if (cur == 1) {
            ensure_seq_capacity(1);
            seq_arr[0] = (seq_t)1;
            seq_len   = 1;
            ensure_cnt_capacity(1);
            ++cnt_arr[1];
            if (max_val < 1) max_val = 1;
            continue;
        }

        /* determine a(cur) from a[1…cur‑1] */
        idx_t m = cur - 1;                  /* length of the prefix       */
        idx_t coincidences = 0;

        /* traverse the prefix while maintaining the sorted multiset */
        idx_t val = max_val;
        idx_t rem = cnt_arr[val];

        for (idx_t i = 0; i < m; ++i) {
            seq_t orig = seq_arr[i];

            /* move to next smaller value if necessary */
            if (rem == 0) {
                do {
                    if (val == 0) break;    /* safety guard */
                    --val;
                } while (val > 0 && cnt_arr[val] == 0);
                if (val > 0) rem = cnt_arr[val];
            }

            seq_t sorted_val = (seq_t)val;
            if (orig == sorted_val) ++coincidences;

            if (rem > 0) --rem;
        }

        /* store the new value and update structures */
        ensure_seq_capacity(cur);
        seq_arr[seq_len] = (seq_t)coincidences;
        ++seq_len;

        idx_t new_val = coincidences;
        ensure_cnt_capacity(new_val);
        ++cnt_arr[new_val];
        if (new_val > max_val) max_val = new_val;
    }

    return seq_arr[n - 1];
}
