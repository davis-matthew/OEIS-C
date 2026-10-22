/*
 * A329866  Numbers whose binary expansion has its runs-resistance equal to
 *          its cuts-resistance minus 1.
 * https://oeis.org/A329866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by search: numbers whose binary runs‑resistance equals cuts‑resistance minus 1 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

/* Return true if value satisfies runs‑resistance = cuts‑resistance – 1 */
static bool qualifies(seq_t value)
{
    idx_t blen = bitlen_seq_t(value);
    if (blen == 0)
        return false;                 /* value 0 is not considered */

    /* Maximum possible number of runs: bit length */
    const size_t MAX_RUNS = 512;
    size_t run_len[MAX_RUNS];
    unsigned char run_bit[MAX_RUNS];
    size_t run_cnt = 0;

    /* Build run lengths and bit values.  The most significant bit is 1. */
    unsigned char prev_bit = 1;
    size_t cur_len = 1;
    for (int pos = (int)blen - 2; pos >= 0; --pos) {
        unsigned char cur_bit = ((value >> pos) & (seq_t)1) ? 1 : 0;
        if (cur_bit == prev_bit) {
            ++cur_len;
        } else {
            if (run_cnt >= MAX_RUNS)
                return false;           /* too many runs – safety */
            run_len[run_cnt] = cur_len;
            run_bit[run_cnt] = prev_bit;
            ++run_cnt;
            cur_len = 1;
            prev_bit = cur_bit;
        }
    }
    if (run_cnt >= MAX_RUNS)
        return false;
    run_len[run_cnt] = cur_len;
    run_bit[run_cnt] = prev_bit;
    ++run_cnt;

    /* Compute max run length for cuts‑resistance upper bound */
    size_t max_run = 0;
    for (size_t i = 0; i < run_cnt; ++i)
        if (run_len[i] > max_run)
            max_run = run_len[i];

    /* Runs‑resistance */
    size_t runs_res;
    if (run_cnt == 1) {
        runs_res = (run_len[0] == 1 ? 0 : 1);
    } else {
        /* initial application from bits to run‑lengths */
        runs_res = 1;
        size_t a[MAX_RUNS], n[MAX_RUNS];
        size_t sz = run_cnt;
        for (size_t i = 0; i < sz; ++i)
            a[i] = run_len[i];
        while (sz > 1) {
            size_t ns = 0;
            for (size_t i = 0; i < sz; ) {
                size_t j = i;
                while (j < sz && a[j] == a[i])
                    ++j;
                n[ns++] = j - i;
                i = j;
            }
            for (size_t i = 0; i < ns; ++i)
                a[i] = n[i];
            sz = ns;
            ++runs_res;
        }
    }

    /* Cuts‑resistance */
    size_t cuts_res = 0;
    size_t cur_sz = run_cnt;
    size_t cur_len_arr[MAX_RUNS];
    unsigned char cur_bit_arr[MAX_RUNS];
    for (size_t i = 0; i < cur_sz; ++i) {
        cur_len_arr[i] = run_len[i];
        cur_bit_arr[i] = run_bit[i];
    }

    while (cur_sz > 0) {
        ++cuts_res;
        /* Decrement all run lengths by one, discarding zeros */
        size_t next_len[MAX_RUNS];
        unsigned char next_bit[MAX_RUNS];
        size_t next_sz = 0;
        for (size_t i = 0; i < cur_sz; ++i) {
            if (cur_len_arr[i] > 1) {
                next_len[next_sz]   = cur_len_arr[i] - 1;
                next_bit[next_sz]   = cur_bit_arr[i];
                ++next_sz;
            }
        }
        /* Merge adjacent runs of the same bit value */
        size_t merged_sz = 0;
        for (size_t i = 0; i < next_sz; ++i) {
            if (merged_sz > 0 && next_bit[merged_sz - 1] == next_bit[i]) {
                cur_len_arr[merged_sz - 1] += next_len[i];
            } else {
                cur_len_arr[merged_sz] = next_len[i];
                cur_bit_arr[merged_sz] = next_bit[i];
                ++merged_sz;
            }
        }
        cur_sz = merged_sz;
    }

    return runs_res == cuts_res - 1;
}

/* ---- Cache for the sequence ---- */
static seq_t *cache    = NULL;
static idx_t  cache_n  = 0;   /* number of terms stored */
static idx_t  cache_sz = 0;   /* allocation size */
static seq_t  current  = 0;   /* last integer examined */

/* Ensure cache can hold at least 'need' elements */
static void ensure_cache(idx_t need)
{
    if (cache_sz < need) {
        size_t new_size = (size_t)need;
        if (new_size < 4)
            new_size = 4;
        else if (new_size < cache_sz * 2)
            new_size = cache_sz * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_sz = new_size;
    }
}

/* Return the n-th term (1‑based) of sequence A329866 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* offset 1 – sentinel for n=0 */

    if (n > cache_n) {
        ensure_cache(n);
        while (cache_n < n) {
            ++current;
            if (qualifies(current))
                cache[cache_n++] = current;
        }
    }
    return cache[n - 1];
}
