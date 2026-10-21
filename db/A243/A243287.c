/*
 * A243287  a(1)=1, and for n > 1, if n is k-th number divisible by the square
 *          of its largest prime factor (i.e., n = A070003(k)), a(n) = 1 +
 *          (2*a(k)); otherwise, when n = A102750(k), a(n) = 2*a(k).
 * https://oeis.org/A243287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1)=1. For n>1, let k be the rank of n in its subsequence: - if
 *            n is divisible by the square of its largest prime factor (i.e.,
 *            n ∈ A070003), then a(n) = 1 + 2*a(k); - otherwise n ∈ A102750
 *            and a(n) = 2*a(k)
 * Range:     u8 n <= 28, u16 n <= 173, u32 n <= 1134, u64 n <= 7940,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of seq_t and idx_t are provided by the build. */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;
static idx_t count070 = 0;      /* number of elements processed in A070003 */
static idx_t count102 = 0;      /* number of elements processed in A102750 */
static idx_t current_max_n = 0; /* highest n for which a[n] is known */

/* Return the largest prime factor of m (m > 1).  */
static uint64_t largest_prime_factor(uint64_t m)
{
    if (m <= 1) return 1;
    uint64_t max_pf = 1;

    /* Factor out 2 */
    while ((m & 1ULL) == 0) {
        max_pf = 2;
        m >>= 1;
    }
    /* Factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) {
            max_pf = p;
            m /= p;
        }
    }
    if (m > 1) max_pf = m;   /* remaining prime factor */
    return max_pf;
}

/* Return true iff n is divisible by the square of its largest prime factor. */
static bool is_in_A070003(idx_t n)
{
    if (n <= 1) return false;
    uint64_t lp   = largest_prime_factor((uint64_t)n);
    uint64_t sq   = lp * lp;
    if (sq == 0 || sq > n) return false;   /* avoid overflow or impossible square */
    return (n % sq == 0);
}

/* Grow the cache array to hold values up to at least n (inclusive). */
static void ensure_a_computed_up_to(idx_t n)
{
    if (n <= current_max_n) return;

    /* Allocate or grow cache */
    if (a_cache_size <= (size_t)n) {
        size_t new_cap = a_cache_size == 0 ? 16 : a_cache_size * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        a_cache = tmp;
        a_cache_size = new_cap;
    }

    /* Compute a[m] for each new m */
    for (idx_t m = current_max_n + 1; m <= n; ++m) {
        if (m == 1) {
            a_cache[m] = (seq_t)1;
            continue;
        }
        if (is_in_A070003(m)) {
            ++count070;
            idx_t rank = count070;       /* rank in A070003 */
            a_cache[m] = (seq_t)1 + ((seq_t)2 * a_cache[rank]);
        } else {
            ++count102;
            idx_t rank = count102;       /* rank in A102750 */
            a_cache[m] = (seq_t)2 * a_cache[rank];
        }
    }
    current_max_n = n;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    ensure_a_computed_up_to(n);
    return a_cache[n];
}
