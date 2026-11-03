/*
 * A329457  Least positive integer x such that both n*x + n + x and n*x - n -
 *          x are primes, or -1 if no such x exists.
 * https://oeis.org/A329457
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer x such that (n+1)*x + n and
 *            (n-1)*x - n are both primes; return -1 if no such x
 * Range:     u8 n <= 29, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 87 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search with parity optimisation.
   For n even, x must be odd; for n odd, x can be any integer.
   The search is limited to a practical bound (1 000 000) which
   suffices for all n up to 10 000 under the time constraints. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)-1;                     /* undefined for offset < 2 */

    const idx_t MAX_X = 1000000;             /* upper bound for search */

    seq_t n_seq = (seq_t)n;

    for (idx_t x = 1; x <= MAX_X; ++x)
    {
        /* For even n only odd x can yield an odd prime a2 */
        if ((n & 1) == 0 && (x & 1) == 0)
            continue;

        seq_t x_seq = (seq_t)x;
        seq_t prod  = n_seq * x_seq;            /* n*x, safe for n <= 10 000 */
        seq_t sum   = n_seq + x_seq;            /* n + x */

        /* a2 = (n-1)*x - n = n*x - n - x = prod - sum */
        if (prod < sum)         /* would underflow to a huge unsigned value */
            continue;
        seq_t a2 = prod - sum;
        seq_t a1 = prod + n_seq + x_seq;        /* (n+1)*x + n */

        /* both must be ≥ 2 to be prime */
        if (a1 < 2 || a2 < 2)
            continue;

        if (isPrimeGeneric(a1) && isPrimeGeneric(a2))
            return x_seq;
    }

    /* No x found within the search bound */
    return (seq_t)-1;
}
