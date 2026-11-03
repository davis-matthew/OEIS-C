/*
 * A329041  Square array read by antidiagonals: A(n, k) = A327936(A276086(n) *
 *          A276086(k)).
 * https://oeis.org/A329041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10439, u16 n <= 10439, u32 n <= 10439, u64 n <= 10439,
 *            u128 n <= 10439, big any n
 * Validated: standard, pass, 121 terms through n = 9700 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = product of primes at positions where the sum of the
 * primorial-base digits of the pair (i, j) (obtained from the n-th
 * antidiagonal index) is ≥ the radix for that position.  The pair (i, j)
 * satisfies i + j = s, where s is the unique integer with
 * s(s+1)/2 ≤ n < (s+1)(s+2)/2.  The antidiagonal index is then
 * (i, j) = (s – (n – s(s+1)/2), n – s(s+1)/2).  Thus the function
 * computes a(n) exactly as defined in OEIS A329041. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>               /* for size_t */

/* Find s such that s(s+1)/2 <= n < (s+1)(s+2)/2.  Uses integer arithmetic. */
static idx_t
find_diagonal_start(idx_t n)
{
    /* Binary search over s in [0, MAX_S] where MAX_S is big enough for n. */
    idx_t low = 0;
    idx_t high = 1;
    while ((high * (high + 1) / 2) <= n) high <<= 1;   /* exponential search */

    while (low < high) {
        idx_t mid = (low + high) / 2;
        if ((mid * (mid + 1) / 2) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Maximum limit for pre‑computed prime list – sufficient for n up to 10⁵. */
#define PRIME_LIMIT 20000

/* Static storage for prime numbers */
static uint32_t PRIMES[PRIME_LIMIT];
static size_t   NUM_PRIMES = 0;

/* --------------------------------------------------------------------- */
/* Simple sieve of Eratosthenes to generate primes up to PRIME_LIMIT.     */
static void
init_primes(void)
{
    if (NUM_PRIMES > 0) return;                     /* already done */

    bool marked[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) marked[i] = false;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!marked[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                marked[j] = true;
        }
    }
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!marked[i]) {
            PRIMES[idx++] = (uint32_t)i;
            if (idx == PRIME_LIMIT) break;
        }
    }
    NUM_PRIMES = idx;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Main function implementing OEIS A329041. */
seq_t
a(idx_t n)
{
    init_primes();                      /* make sure PRIMES is ready */

    /* Determine pair (i, j) from the antidiagonal index n. */
    idx_t s = find_diagonal_start(n);               /* s >= 0 */
    idx_t diag_start = s * (s + 1) / 2;             /* first index of diag s */
    idx_t pos = n - diag_start;                     /* position within diagonal */
    idx_t i = s - pos;
    idx_t j = pos;

    seq_t result = 1;

    /* Walk through primorial digits; stop when both i and j are zero. */
    idx_t idx = 0;                                   /* position index */
    while ((i > 0) || (j > 0) || (idx < NUM_PRIMES)) {
        uint64_t radix = PRIMES[idx];             /* base of current digit */
        seq_t di = (seq_t)(i % radix);
        seq_t dj = (seq_t)(j % radix);
        if ((di + dj) >= radix)
            result *= (seq_t)radix;

        i /= radix;
        j /= radix;
        ++idx;
        /* We can stop early if both i and j are zero and we have processed
         * all remaining radices; but we leave the loop as is because
         * idx will eventually exceed NUM_PRIMES and exit. */

        if (i == 0 && j == 0 && idx > NUM_PRIMES) break;
    }

    return result;
}
