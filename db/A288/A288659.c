/*
 * A288659  Numbers whose cubes have an odd number of digits.
 * https://oeis.org/A288659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 742, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A288659
   Numbers whose cubes have an odd number of digits.
   The sequence starts:
      0, 1, 2, 5, 6, 7, 8, 9, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, ...
   a(n) is the n‑th term (n≥1).  This implementation memoises the values so that
   calls may come in any order. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Return the number of decimal digits of a seq_t value.
   Implemented here to avoid any utility header dependency. */
static int digit_len_seq(seq_t x)
{
    seq_t ten = (seq_t)10;
    int len = 0;
    if (x == (seq_t)0)
        return 1;
    while (x > (seq_t)0) {
        x = x / ten;
        len++;
    }
    return len;
}

static seq_t *cache        = NULL;   /* memoised sequence values           */
static size_t cache_size   = 0;       /* number of values stored            */
static size_t cache_cap    = 0;       /* current allocation capacity        */
static idx_t  next_number  = 0;       /* integer to test next (starts at 0) */

/* Public routine: returns the n-th element of A288659 for n≥1. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n values stored. */
    while (cache_size < (size_t)n) {
        idx_t            m      = next_number++;
        seq_t            m_seq  = (seq_t)m;
        seq_t            cube   = m_seq * m_seq;
        cube = cube * m_seq; /* cube = m_seq^3 */
        int digits = digit_len_seq(cube);
        if (digits & 1) { /* odd number of digits */
            if (cache_size == cache_cap) { /* grow the array */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* Out of memory – aborting would be simplest for this
                       context; the test harness should never hit this. */
                    abort();
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = m_seq;
        }
    }
    return cache[(size_t)(n - (idx_t)1)];
}
