/*
 * A400364  Sum of the fourth powers of the legs of the primitive Pythagorean
 *          triangle with prime hypotenuse A002144(n).
 * https://oeis.org/A400364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = A002365(n)^4 + A002366(n)^4 (sum of the fourth powers of
 *            the legs of the primitive Pythagorean triangle with prime
 *            hypotenuse A002144(n))
 * Range:     u8 none, u16 n <= 3, u32 n <= 25, u64 n <= 2549, u128 n <= 2549,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* ---------- Helper: simple primality test for uint64_t ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) return false;
    }
    return true;
}

/* ---------- Helper: find representation p = x² + y² (x > y > 0) ---------- */
static bool find_xy(uint64_t p, uint64_t *out_x, uint64_t *out_y)
{
    idx_t limit = (idx_t)floor_sqrt((seq_t)p);
    for (idx_t y = 1; y <= limit; ++y) {
        seq_t y2   = (seq_t)y * y;
        seq_t remain = (seq_t)p - y2;
        seq_t x = floor_sqrt(remain);
        if (x * x == remain && x > (seq_t)y) {
            *out_x = (uint64_t)x;
            *out_y = (uint64_t)y;
            return true;
        }
    }
    return false;
}

/* ---------- 1‑mod‑4 prime generation (cached) ---------- */
static uint64_t *primes_1mod4 = NULL;  /* dynamic array of primes ≡ 1 (mod 4) */
static idx_t primes_used  = 0;        /* number of primes stored   */
static idx_t primes_cap   = 0;        /* capacity of the array     */
static uint64_t last_checked = 3;     /* last odd number examined   */

static uint64_t nth_prime_1mod4(idx_t n)
{
    while (primes_used < n) {
        last_checked += 2;               /* examine only odd numbers   */
        if (last_checked % 4ULL != 1ULL) continue;
        if (is_prime_uint64(last_checked)) {
            if (primes_used == primes_cap) {
                size_t new_cap = primes_cap < 4 ? 4 : primes_cap * 2;
                uint64_t *tmp = (uint64_t*)realloc(primes_1mod4,
                                                   new_cap * sizeof(uint64_t));
                if (!tmp) abort();        /* allocation failure */
                primes_1mod4 = tmp;
                primes_cap   = new_cap;
            }
            primes_1mod4[primes_used++] = last_checked;
        }
    }
    return primes_1mod4[n - 1];
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* 1. Get the n‑th prime ≡ 1 (mod 4). */
    uint64_t p = nth_prime_1mod4(n);

    /* 2. Compute the Legendre representation p = x² + y². */
    uint64_t x, y;
    if (!find_xy(p, &x, &y))
        return (seq_t)0;   /* Should never happen */

    /* 3. Compute the legs u = x² – y², v = 2xy. */
    seq_t xx = (seq_t)x * (seq_t)x;
    seq_t yy = (seq_t)y * (seq_t)y;
    seq_t u  = xx - yy;                      /* u = x² – y²   */
    seq_t v  = (seq_t)2 * (seq_t)x * (seq_t)y; /* v = 2 xy      */

    /* 4. Return u⁴ + v⁴. */
    seq_t u2 = u * u;
    seq_t u4 = u2 * u2;
    seq_t v2 = v * v;
    seq_t v4 = v2 * v2;

    return u4 + v4;
}
