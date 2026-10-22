/*
 * A300356  Number of ways to write n^2 as x^2 + y^2 + z^2 + w^2 with x >= y
 *          >= 0 <= z <= w such that x + 63*y = 2^(2k+1) for some nonnegative
 *          integer k.
 * https://oeis.org/A300356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5153, u16 n <= 6000, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A300356
 *
 * a(n) counts the quadruples (x,y,z,w) of non‑negative integers with
 * x >= y >= 0,  z <= w,  x² + y² + z² + w² = n²,  and
 * x + 63·y = 2^(odd exponent).  The function enumerates all admissible
 * (x,y) satisfying the linear condition and, for each, counts the
 * unordered non‑negative solutions (z,w) of the remaining two‑square
 * equation.
 *
 * The implementation uses an O(number of (x,y) pairs · sqrt(n)) routine
 * which is fast enough for n up to about 10 000.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/* Small integer square‑root for 64‑bit values (binary search). */
static uint32_t floor_sqrt_uint64(uint64_t n)
{
    uint32_t low = 0, high = 100000;        /* 1e5² > 1e10 > 1e8 */
    while (low <= high) {
        uint32_t mid = (low + high) >> 1;
        uint64_t msq = (uint64_t)mid * mid;
        if (msq <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* ------------------------------------------------------------ */
/* Prime table up to 10 000, generated once. */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes != NULL) return;

    const size_t LIM = 10000;
    bool is_comp[LIM + 1];
    memset(is_comp, 0, sizeof(is_comp));

    size_t cnt = 0;
    for (size_t i = 2; i <= LIM; ++i) {
        if (!is_comp[i]) {
            ++cnt;
            if (i * i <= LIM) {
                for (size_t j = i * i; j <= LIM; j += i)
                    is_comp[j] = true;
            }
        }
    }

    uint64_t *arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIM; ++i)
        if (!is_comp[i])
            arr[idx++] = i;

    primes = arr;
    prime_cnt = cnt;
}

/* ------------------------------------------------------------ */
/* Count unordered non‑negative integer solutions (z,w) to
   z² + w² = R.  The function returns 1 for R = 0 (0,0). */
static seq_t count_two_squares(uint64_t R)
{
    if (R == 0) return (seq_t)1;

    uint64_t temp = R;
    uint64_t prod = 1;          /* product over primes ≡ 1 (mod 4) of (e+1) */
    init_primes();

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);

            if ((p % 4 == 3) && (exp & 1))
                return (seq_t)0;          /* no representations */

            if (p % 4 == 1)
                prod *= (exp + 1);
        }
    }
    if (temp > 1) {                         /* remaining prime factor */
        uint64_t p = temp;
        if (p % 4 == 3) return (seq_t)0;
        if (p % 4 == 1) prod *= 2;
    }

    /* r₂(R) = 4 * prod  (number of integer solutions with signs & order) */
    uint64_t r2 = 4 * prod;

    /* c0 = 1 if R is a perfect square (x = sqrt(R), y = 0) */
    uint32_t sq = floor_sqrt_uint64(R);
    int c0 = ((uint64_t)sq * (uint64_t)sq == R) ? 1 : 0;

    /* c2 = 1 if R/2 is a perfect square (x = y = sqrt(R/2)) */
    int c2 = 0;
    if ((R & 1ULL) == 0) {
        uint64_t half = R >> 1;
        uint32_t sh = floor_sqrt_uint64(half);
        if ((uint64_t)sh * (uint64_t)sh == half) c2 = 1;
    }

    return (seq_t)((r2 + 4ULL * (c0 + c2)) / 8ULL);
}

/* ------------------------------------------------------------ */
/* Main sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_primes();

    uint64_t n64 = (uint64_t)n;
    uint64_t nSq = n64 * n64;
    uint64_t maxP = 64ULL * n64;          /* largest possible p = x + 63*y */

    seq_t total = 0;

    for (unsigned int exp = 1; exp < 64 && ((1ULL << exp) <= maxP); exp += 2) {
        uint64_t p = 1ULL << exp;
        uint64_t y_max = p / 63ULL;
        if (y_max > n64) y_max = n64;

        for (uint64_t y = 0; y <= y_max; ++y) {
            uint64_t x = p - 63ULL * y;
            if (x < y) continue;          /* enforce x >= y */
            if (x > n64) continue;        /* x must fit in n² */

            uint64_t xy2 = x * x + y * y;
            if (xy2 > nSq) continue;

            uint64_t rem = nSq - xy2;
            total += count_two_squares(rem);
        }
    }
    return total;
}
