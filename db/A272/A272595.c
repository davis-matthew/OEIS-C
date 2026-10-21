/*
 * A272595  Numbers n such that the multiplicative group modulo n is the
 *          direct product of 5 cyclic groups.
 * https://oeis.org/A272595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A272595 – Numbers n such that the multiplicative
   group modulo n is the direct product of exactly 5 cyclic groups.
   The function returns the nth term for an index of at least 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"

static seq_t *seq_cache = NULL;   /* cached terms */
static size_t seq_len   = 0;       /* number of cached terms */
static size_t seq_cap   = 0;       /* capacity of seq_cache */
static uint64_t next_check = 2;    /* next integer to test */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Return true iff the multiplicative group modulo n has exactly
   5 cyclic direct factors. */
static bool qualifies(uint64_t n)
{
    /* Count exponent of 2. */
    int e2 = 0;
    while ((n & 1ULL) == 0) {
        ++e2;
        n >>= 1U;
    }

    /* Determine how many cyclic factors arise from the 2‑part. */
    size_t f2 = 0;
    if      (e2 == 2) f2 = 1;
    else if (e2 >= 3) f2 = 2;

    /* Number of odd prime powers required. */
    size_t required = 5 - f2;
    if (required == 0 && n == 1) return true; /* only n=1 would qualify when 5=0 */

    /* Factor the remaining odd part (n may now be >1 or ==1). */
    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t primes_cnt = 0;
    factor_u64(n, primes_buf, exps_buf, &primes_cnt);

    return primes_cnt == required;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for index 0 */

    size_t target = (size_t)n;

    /* Generate terms until we have 'target' entries. */
    while (seq_len < target) {
        uint64_t cur = next_check++;
        if (qualifies(cur)) {
            ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)cur;
        }
    }

    return seq_cache[n - 1];
}
