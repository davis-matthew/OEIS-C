/*
 * A392454  a(n) = Sum_{k=0..floor(2*n/3)} binomial(2*k+1,2*n-3*k).
 * https://oeis.org/A392454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(2*n/3)} binomial(2*k+1, 2*n-3*k) *
 *            Recurrence: a(n) = 4*a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), n >= 5
 *            * Initial values: a(0)=1, a(1)=0, a(2)=3, a(3)=2, a(4)=10,
 *            a(5)=12
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed < 64 ? 64 : needed;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();      /* out of memory */
    seq_cache = tmp;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    if (cache_len > 0)
        return;
    cache_len = 6;          /* indices 0..5 */
    seq_cache[0] = (seq_t)1;
    seq_cache[1] = (seq_t)0;
    seq_cache[2] = (seq_t)3;
    seq_cache[3] = (seq_t)2;
    seq_cache[4] = (seq_t)10;
    seq_cache[5] = (seq_t)12;
}

seq_t a(idx_t n)
{
    if (n <= 5) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)0;
            case 2: return (seq_t)3;
            case 3: return (seq_t)2;
            case 4: return (seq_t)10;
            case 5: return (seq_t)12;
        }
    }

    ensure_cache_capacity(n);
    init_cache();

    while (cache_len <= n) {
        idx_t i = cache_len;
        seq_t val = (seq_t)4 * seq_cache[i-2]
                  + seq_cache[i-3]
                  - (seq_t)2 * seq_cache[i-4]
                  + seq_cache[i-5];
        seq_cache[cache_len++] = val;
    }

    return seq_cache[n];
}
