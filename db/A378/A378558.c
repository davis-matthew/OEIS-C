/*
 * A378558  Powers of 4 that do not include the digit 4.
 * https://oeis.org/A378558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   digit_counts, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) returns the nth power of 4 whose decimal representation
 *              contains no digit 4 by sequentially testing powers of 4. */
#include <stddef.h>        /* size_t */
#include <stdint.h>        /* uint64_t, etc. */
#include <stdbool.h>       /* bool */

#include "../utils/digit_counts.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;      /* cached valid powers of 4 */
static size_t cache_size = 0;    /* number of cached elements */
static size_t cache_cap  = 0;    /* allocated capacity of cache */

static seq_t current_value = (seq_t)1;  /* 4^k, starting with k = 0 */

/* Returns the nth term (1‑based) of the sequence of powers of 4 with no
 * digit 4 in its decimal representation.  The function is memoised and
 * works for any order of calls. */
seq_t a(idx_t n) {
    size_t target = (size_t)n;           /* n is 1‑based; cast for array index */

    /* Generate terms until we have at least 'n' of them. */
    while (cache_size < target) {
        unsigned char cnt[10] = {0};

        /* Count digits of current_value; cnt[4] will be zero if digit 4 is absent. */
        digit_counts(current_value, cnt);

        if (cnt[4] == 0) {
            /* Store the term, growing the cache as required. */
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = current_value;
        }

        /* Prepare the next power of 4, stopping if an overflow would occur
         * for integral seq_t types.  For arbitrary‑precision types the
         * multiplication never overflows. */
        if (sizeof(seq_t) <= 8) {          /* integral type (uint8_t…uint64_t) */
            /* Detect potential overflow before multiplying. */
            if (current_value > (((seq_t)-1) / (seq_t)4))
                break;  /* cannot multiply without loss of information */
            current_value = current_value * (seq_t)4;
        } else {
            current_value = current_value * (seq_t)4;  /* big‑int type */
        }
    }

    /* The requested term now must exist (problem guarantees this). */
    return cache[n - 1];
}
