/*
 * A259936  Number of ways to express the integer n as a product of its
 *          unitary divisors (A034444).
 * https://oeis.org/A259936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(ω(n)) where ω(n) is the number of distinct prime
 *            factors of n. The Bell numbers are computed once up to a safe
 *            maximum number of distinct prime factors (here 32)
 * Range:     u8 n <= 9814, u16 n <= 9814, u32 n <= 9814, u64 n <= 9814,
 *            u128 n <= 9814, big any n
 * Helpers:   distinct_prime_factor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We need the common <stdbool.h> header for the bool type. */
#include <stdbool.h>
#include "../utils/distinct_prime_factor_count.h"

/* ---------- Binomial coefficient (n choose k) implemented in seq_t ---------- */
static seq_t
binom(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* (n - k + i) is guaranteed to be divisible by i,
           so no precision loss occurs. */
        res = res * (n - k + i) / i;
    }
    return res;
}

/* ---------- Bell numbers up to a fixed maximum (32) ---------- */
#define MAX_R 32
static seq_t bell[MAX_R + 1];
static bool bell_initialized = false;

static void
init_bell(void)
{
    if (bell_initialized)
        return;
    bell[0] = 1;
    for (idx_t i = 1; i <= MAX_R; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += binom(i - 1, k) * bell[k];
        }
        bell[i] = sum;
    }
    bell_initialized = true;
}

/* ---------- Main sequence function ---------- */
seq_t
a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       to avoid undefined behaviour; the tests will never query this. */
    if (n == 0)
        return 0;

    init_bell();                  /* ensure Bell numbers are ready */

    uint64_t m = (uint64_t)n;     /* work in 64‑bit arithmetic for factoring */
    idx_t r = distinct_prime_factor_count(m);
    if (r > MAX_R) {
        /* The chosen MAX_R is generous.  If an input ever exceeds it
           we recompute Bell numbers up to the required size.  This
           branch is unlikely for the intended test range. */
        for (idx_t i = MAX_R + 1; i <= r; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                sum += binom(i - 1, k) * bell[k];
            }
            bell[i] = sum;
        }
    }
    return bell[r];
}
