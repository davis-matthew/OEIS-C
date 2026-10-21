/*
 * A245642  Sum of "number of decompositions of d into ordered sums of two odd
 *          primes" over all divisors d of 2*n.
 * https://oeis.org/A245642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|2n, d even} (#ordered decompositions of d into two
 *            odd primes)
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  For each even divisor d, the number of ordered decompositions equals 2*A002372(d) */
/*  minus 1 if d is twice an odd prime.                                           */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*  Global state for memoisation  */
static seq_t   *unordered_counts = NULL;   /* A002372 values for sums up to current max */
static bool    *is_prime_flag    = NULL;   /* sieve array for primes up to current max */
static uint64_t *odd_primes      = NULL;   /* list of odd primes up to current max */
static size_t    prime_cnt       = 0;
static size_t    max_limit       = 0;       /* maximum n*2 processed so far */

/*  Rebuild all data up to at least 'limit' (inclusive)  */
static void
rebuild_up_to(size_t limit)
{
    if (limit <= max_limit)
        return;                         /* already up-to-date */

    /*  Free old structures  */
    free(unordered_counts);
    free(is_prime_flag);
    free(odd_primes);

    max_limit = limit;

    unordered_counts = (seq_t*)malloc((max_limit + 1) * sizeof(seq_t));
    is_prime_flag    = (bool*)malloc((max_limit + 1) * sizeof(bool));
    if (!unordered_counts || !is_prime_flag)
        return;                         /* out of memory – caller will see 0 */

    /*  Sieve of Eratosthenes for odd primes only  */
    for (size_t i = 0; i <= max_limit; ++i)
        is_prime_flag[i] = true;
    if (max_limit >= 0) is_prime_flag[0] = false;
    if (max_limit >= 1) is_prime_flag[1] = false;
    for (size_t i = 2; i * i <= max_limit; ++i)
        if (is_prime_flag[i])
            for (size_t j = i * i; j <= max_limit; j += i)
                is_prime_flag[j] = false;

    /*  Build list of odd primes  */
    size_t prime_cap = max_limit / 2 + 10;          /* enough for odd primes */
    odd_primes = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    prime_cnt  = 0;
    for (size_t i = 3; i <= max_limit; i += 2)
        if (is_prime_flag[i])
            odd_primes[prime_cnt++] = (uint64_t)i;

    /*  Initialise unordered counts  */
    for (size_t i = 0; i <= max_limit; ++i)
        unordered_counts[i] = (seq_t)0;

    /*  Compute unordered Goldbach partitions for every even sum up to max_limit */
    for (size_t a = 0; a < prime_cnt; ++a) {
        uint64_t p = odd_primes[a];
        for (size_t b = a; b < prime_cnt; ++b) {
            uint64_t q = odd_primes[b];
            uint64_t s = p + q;
            if (s > (uint64_t)max_limit)
                break;
            unordered_counts[(size_t)s] += (seq_t)1;
        }
    }
}

/*  Number of ordered decompositions of an even integer d into two odd primes  */
static seq_t
ordered_count(idx_t d)
{
    /*  d must be even; if not, count is 0  */
    if (d & 1)
        return (seq_t)0;

    seq_t res = unordered_counts[(size_t)d];  /* unordered count */
    res = res + res;                         /* double for ordering */

    idx_t half = d >> 1;                     /* d = 2 * half */
    /*  Subtract the single (p,p) case if half is an odd prime  */
    if (half >= 3 && half <= (idx_t)max_limit && is_prime_flag[(size_t)half] && (half & 1))
        res -= (seq_t)1;

    return res;
}

/*  External function required by the specification  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* safety guard */

    idx_t m = n << 1;                        /* 2*n */

    rebuild_up_to((size_t)m);                /* ensure data up to m */

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t d1 = i;                    /* first divisor  */
            idx_t d2 = m / i;                /* paired divisor */

            if ((d1 & 1) == 0)
                sum += ordered_count(d1);

            if (d2 != d1 && (d2 & 1) == 0)
                sum += ordered_count(d2);
        }
    }
    return sum;
}
