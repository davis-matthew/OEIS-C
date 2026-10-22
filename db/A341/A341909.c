/*
 * A341909  a(0) = 0; for n > 0, a(n) is the smallest positive integer not yet
 *          in the sequence such that the first digit of a(n) differs by 1
 *          from the last digit of a(n-1).
 * https://oeis.org/A341909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10_ssize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A341909  
 * a(0) = 0
 * for n > 0:  a(n) is the smallest positive integer not yet in the sequence
 *              such that the first digit of a(n) differs by ±1 from
 *              the last digit of a(n-1).  The algorithm keeps a cache of
 *              already generated terms and a per‑first‑digit “next candidate”
 *              pointer so that any call a(n) (in any order) runs in
 *              amortised O(1) time.  The implementation works for all
 *              supported seq_t types (small integer or arbitrary‑precision). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10_ssize.h"

static seq_t *cache          = NULL;     /* cached sequence terms   */
static size_t cache_cap      = 0;         /* capacity                 */
static size_t cache_len      = 0;         /* number of terms cached   */
static bool   initialized    = false;     /* initialization flag      */
static seq_t  pos[10];                    /* next unused number for first digit 1..9 */

/* Ensure the cache can hold at least 'cap' elements.  Returns the new capacity. */
static size_t ensure_cache_capacity(size_t cap)
{
    if (cap <= cache_cap) return cache_cap;
    size_t new_cap = cap > 1 ? cap : 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();               /* allocation failure */
    cache   = new_cache;
    cache_cap = new_cap;
    return new_cap;
}

/* Advance pos[f] to the next unused number with first digit f (f in 1..9). */
static void advance_pos(int f)
{
    seq_t cur = pos[f];
    int    d  = digit_len(cur);              /* number of decimal digits */
    unsigned int k  = (unsigned int)d - 1;   /* trailing digits count    */
    seq_t powk = pow10_ssize(k);

    /* (f+1)*10^k - 1  is the largest number in the current block */
    seq_t max_in_block = (seq_t)(f + 1) * powk - 1;
    if (cur == max_in_block) {
        /* Move to the first number of the next block: f * 10^(k+1) */
        seq_t powk1 = powk * 10;              /* 10^(k+1)  */
        pos[f] = (seq_t)f * powk1;
    } else {
        pos[f] = cur + 1;
    }
}

seq_t a(idx_t n)
{
    /* One‑time initialization */
    if (!initialized) {
        for (int f = 1; f <= 9; ++f) pos[f] = (seq_t)f;
        initialized = true;
        ensure_cache_capacity(1);
        cache[0] = 0;
        cache_len = 1;
    }

    /* Extend cache if necessary */
    if ((idx_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        ensure_cache_capacity(new_len);
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t prev = cache[i - 1];
            int   last_digit = (int)(prev % (seq_t)10);

            /* Determine allowed first digits for the new term */
            int allowed[2];
            int cnt = 0;
            if (last_digit == 0) {
                allowed[0] = 1;
                cnt = 1;
            } else {
                if (last_digit - 1 >= 1) allowed[cnt++] = last_digit - 1;
                if (last_digit + 1 <= 9) allowed[cnt++] = last_digit + 1;
            }

            /* Select the smallest candidate among the allowed first digits */
            seq_t best_val = pos[allowed[0]];
            int   best_f   = allowed[0];
            for (int j = 1; j < cnt; ++j) {
                seq_t cand = pos[allowed[j]];
                if (cand < best_val) {
                    best_val = cand;
                    best_f   = allowed[j];
                }
            }

            cache[i] = best_val;
            advance_pos(best_f);
        }
        cache_len = new_len;
    }

    return cache[n];
}
