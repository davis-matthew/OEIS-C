/*
 * A275471  Number of ordered ways to write n as 4^k*(1+x^2+y^2)+z^2, where
 *          k,x,y,z are nonnegative integers with x <= y and x == y (mod 2).
 * https://oeis.org/A275471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by enumerating possible z, k and pre‑computing
 * the number of admissible pairs (x, y) with x≤y and x≡y (mod 2)
 * and x²+y² = t.  The function reuses a static cache so that
 * repeated calls are fast.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root: floor(sqrt(n)) for 0 ≤ n < 2⁶⁴ */
static uint64_t u64_isqrt(uint64_t n)
{
    uint64_t low = 0, high = (n >= 0xFFFFFFFFULL) ? 0xFFFFFFFFULL : (uint64_t)n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Cache of pair counts: rep_cache[t] = number of (x,y) with
 * x ≤ y, x≡y (mod 2) and x² + y² = t.  rep_size is the capacity. */
static seq_t *rep_cache = NULL;
static size_t rep_size  = 0;

/* Initialise or expand the cache so that indices up to max_t are valid. */
static void init_rep(size_t max_t)
{
    if (rep_size > max_t) return;

    size_t new_size = max_t + 1;
    seq_t *tmp = (seq_t *)realloc(rep_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure */
    rep_cache = tmp;

    for (size_t t = rep_size; t < new_size; ++t) {
        seq_t count = 0;
        uint64_t ut   = (uint64_t)t;
        uint64_t max_x = u64_isqrt(ut);
        for (uint64_t x = 0; x <= max_x; ++x) {
            uint64_t x2  = x * x;
            uint64_t rem = ut - x2;
            uint64_t y   = u64_isqrt(rem);
            if (y * y != rem) continue;          /* y² does not match */
            if (y < x) continue;                 /* enforce x ≤ y */
            if ((x & 1ULL) != (y & 1ULL)) continue;  /* parity match */
            count += 1;
        }
        rep_cache[t] = count;
    }
    rep_size = new_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* guard for non‑positive indices */

    /* Ensure that reps up to n are available (t ≤ n‑1 for valid inputs). */
    init_rep((size_t)n);

    seq_t result = 0;
    uint64_t un = (uint64_t)n;

    for (uint64_t z = 0; ; ++z) {
        uint64_t z2 = z * z;
        if (z2 > un) break;
        uint64_t rest = un - z2;          /* need 4^k * restdiv = rest */

        uint64_t power = 1;               /* 4^0 */
        while (power <= rest) {
            if ((rest % power) == 0) {
                uint64_t restdiv = rest / power;   /* must be ≥1 */
                if (restdiv >= 1) {
                    uint64_t t = restdiv - 1;       /* x² + y² = t */
                    if (t < rep_size)
                        result += rep_cache[t];
                }
            }
            /* stop if next power would overflow or exceed rest */
            if (power > rest >> 2) break;
            power <<= 2;                   /* multiply by 4 */
        }
    }
    return result;
}
