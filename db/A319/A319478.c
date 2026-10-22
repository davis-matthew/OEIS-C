/*
 * A319478  a(n) is the least base b > 1 such that the product n * n can be
 *          computed without carry by long multiplication.
 * https://oeis.org/A319478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the least base b>1 such that the square of n can be
 *            multiplied without carry in base b. The function tests all bases
 *            from 2 up to n+1 (or the maximum representable value) and checks
 *            the carry‑free condition
 * Range:     u8 n <= 449, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache of already-computed values; the cache grows to store up to
   the largest n requested. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

static seq_t *ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return NULL; /* Allocation failed */
        /* Zero the new part of the cache */
        for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_size = new_size;
    }
    return cache;
}

seq_t a(idx_t n)
{
    /* Handle 0 explicitly – base 2 is the first carry‑free base for 0. */
    if (n == 0) return (seq_t)2;

    seq_t *c = ensure_cache(n);
    if (!c) return (seq_t)0;          /* Allocation failure protection */
    if (c[n] != (seq_t)0) return c[n];

    /* Convert the index to the sequence type. */
    seq_t nn = (seq_t)n;

    /* Determine the upper bound for the base search.  For n>1 the
       minimal base is guaranteed to be <= n+1, so that is used as a
       safe bound.  If the conversion overflows (n exceeds the
       representable range of seq_t), the bound is capped at the
       maximum representable value. */
    seq_t limit;
    /* Max value representable by seq_t is ~((seq_t)0) */
    seq_t max_seq = ~(seq_t)0;
    idx_t max_seq_idx = (idx_t)max_seq;
    if (n > max_seq_idx) {
        limit = max_seq;
    } else {
        limit = nn + (seq_t)1;
        if (limit < (seq_t)2) limit = (seq_t)2;
    }

    /* Temporary buffers for digits and convolution sums.  The
       maximum number of digits for the problem sizes considered is
       far below 256.  The convolution sums are held in 64‑bit
       unsigned integers to avoid overflow even when seq_t is 8 or
       16 bits wide. */
    static seq_t digits[256];
    static uint64_t conv[512];

    seq_t result = (seq_t)2;   /* Fallback default value */

    for (seq_t b = (seq_t)2; b <= limit; ++b) {
        /* Extract digits of nn in base b. */
        seq_t tmp = nn;
        size_t len = 0;
        if (tmp == (seq_t)0) {
            digits[0] = (seq_t)0;
            len = 1;
        } else {
            while (tmp != (seq_t)0) {
                digits[len++] = tmp % b;
                tmp /= b;
            }
        }

        /* Compute convolution of the digit array with itself. */
        size_t conv_len = 2 * len - 1;
        for (size_t i = 0; i < conv_len; ++i) conv[i] = 0ULL;

        for (size_t i = 0; i < len; ++i) {
            for (size_t j = 0; j < len; ++j) {
                conv[i + j] += (uint64_t)digits[i] * (uint64_t)digits[j];
            }
        }

        /* Check the carry‑free condition: all convolution sums
           must be strictly less than the base b. */
        bool ok = true;
        for (size_t k = 0; k < conv_len; ++k) {
            if (conv[k] >= (uint64_t)b) {
                ok = false;
                break;
            }
        }

        if (ok) {
            result = b;
            break;
        }
    }

    c[n] = result;
    return result;
}
