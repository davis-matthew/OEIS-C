/*
 * A373410  Minimum of the n-th maximal antirun of nonsquarefree numbers
 *          differing by more than one.
 * https://oeis.org/A373410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373410:
   a(1) = 4;
   for n > 1:  a(n) = A068781(n‑1) + 1
   A068781(k) is the maximum value in the k‑th antirun of
   nonsquarefree integers (an antirun is a maximal subsequence of
   nonsquarefree integers where consecutive terms differ by > 1). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
 *  Prime sieve (up to 1 000 000) for quick nonsquarefree test.
 * ------------------------------------------------------------------ */
#define PRIME_LIMIT 1000000u
static uint64_t primes[PRIME_LIMIT / 10 + 1000]; /* about 78498 primes */
static size_t   prime_cnt  = 0;
static bool     primes_ready = false;

static void init_primes(void)
{
    if (primes_ready)
        return;

    static bool composite[PRIME_LIMIT + 1] = {false};

    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= PRIME_LIMIT) {
                for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* ------------------------------------------------------------------
 *  Test whether x is nonsquarefree (divisible by a square > 1).
 * ------------------------------------------------------------------ */
static inline bool is_nonsquarefree(uint64_t x)
{
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p  = primes[idx];
        uint64_t sq = p * p;
        if (sq > x)
            break;
        if (x % sq == 0)
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------
 *  Sequence A373410
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Offset handling */
    if (n < 1)
        return 0;       /* behaviour below the offset is unspecified */

    if (n == 1)
        return (seq_t)4; /* a(1) = 4 */

    /* Prepare prime data for nonsquarefree tests */
    init_primes();

    /* We need the maximum of the (n‑1)-th antirun.  The antiruns are
       counted starting from 1.  antirun_index tracks how many runs
       have already finished.  */
    const idx_t target = n - 1;     /* target antirun (1‑based) */

    /* State of the current run */
    uint64_t previous_ns      = 4; /* first nonsquarefree number */
    uint64_t current_run_max  = 4;
    idx_t    antirun_index    = 0; /* runs finished so far */

    /* Search for the following nonsquarefree numbers */
    uint64_t m = 5; /* start from the next integer */

    for (;;) {
        /* Find next nonsquarefree number */
        while (!is_nonsquarefree(m))
            ++m;
        uint64_t ns = m;
        ++m; /* advance for next iteration */

        if (ns - previous_ns > 1) {
            /* Same run continues */
            current_run_max = ns;
        } else {
            /* Current run ends before ns */
            if (antirun_index + 1 == target)
                return (seq_t)current_run_max + (seq_t)1;
            /* Move to the next run */
            antirun_index++;
            current_run_max = ns;
        }
        previous_ns = ns;
    }
}
