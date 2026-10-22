/*
 * A374897  a(n) is the least semiprime that ends with n, or -1 if there is no
 *          such semiprime.
 * https://oeis.org/A374897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) is the smallest semiprime that ends with the decimal
 *            representation of n, or -1 if no such semiprime exists
 * Range:     i8 n <= 10, i16 n <= 1032, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------
   We use a small trial‑division sieve for primes and the floor_sqrt
   helper from the utils library.  The code works for any integer size
   supplied for seq_t / idx_t and respects the special exclusion
   conditions listed in the OEIS entry.
   -------------------------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* --------------------------------------------------------------------
   Prime table (≤ 1,000,000).  Sufficient for all required factorizations
   when searching up to 10 000.
   -------------------------------------------------------------------- */
#define PRIME_LIMIT   1000000
#define MAX_PRIMES    78700   /* number of primes <= PRIME_LIMIT */

static uint32_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;
static bool primes_built = false;

static void build_primes(void)
{
    if (primes_built) return;
    static uint8_t is_comp[PRIME_LIMIT + 1];
    for (int i = 2; i <= PRIME_LIMIT; ++i) is_comp[i] = 0;
    for (int i = 2; (int64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i) is_comp[j] = 1;
        }
    }
    primes[prime_cnt++] = 2;
    for (int i = 3; i <= PRIME_LIMIT; i += 2)
        if (!is_comp[i]) primes[prime_cnt++] = (uint32_t)i;
    primes_built = true;
}

/* --------------------------------------------------------------------
   Return true iff x is a semiprime (product of exactly two primes,
   counting multiplicities).
   -------------------------------------------------------------------- */
static bool is_semiprime(seq_t x)
{
    if (x < 4) return false;          /* smallest semiprime is 4 = 2*2 */
    seq_t rem = x;
    unsigned cnt = 0;
    seq_t limit = floor_sqrt(rem);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        seq_t sp = p;
        if (sp > limit) break;
        if (rem % sp == 0) {
            while (rem % sp == 0) {
                ++cnt;
                rem /= sp;
                if (cnt > 2) return false;
            }
            if (rem == 1) break;
            limit = floor_sqrt(rem);
        }
    }
    if (rem > 1) ++cnt;
    return cnt == 2;
}

/* --------------------------------------------------------------------
   Main sequence function.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    build_primes();

    /* Apply OEIS exclusion rules. */
    if ((n > 10ULL && ((n % 4ULL) == 0ULL || (n % 10ULL) == 0ULL)) ||
        (n > 25ULL && (n % 25ULL) == 0ULL))
        return (seq_t)-1;

    /* Number of decimal digits of n. */
    idx_t tmp = n;
    unsigned digits = 1;
    while (tmp >= 10ULL) {
        tmp /= 10ULL;
        ++digits;
    }

    /* 10^digits as seq_t. */
    seq_t step = 1;
    for (unsigned i = 0; i < digits; ++i) step *= 10;

    /* First candidate with the required suffix. */
    seq_t candidate = (seq_t)n;
    /* Ensure candidate is at least 4 (the smallest semiprime). */
    if (candidate < 4) {
        /* Increase by multiples of step until >=4. */
        unsigned inc = (unsigned)((4 - candidate + step - 1) / step);
        candidate += step * inc;
    }

    for (;;) {
        if (is_semiprime(candidate))
            return candidate;
        candidate += step;
    }
}
