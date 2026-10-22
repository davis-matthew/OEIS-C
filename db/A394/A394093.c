/*
 * A394093  Greatest 0-based partial alternating sum of the prime indices of
 *          n.
 * https://oeis.org/A394093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A394093 – Greatest 0‑based partial alternating sum of
 *  the prime indices of n
 *
 *  a(1) = 0
 *  a(n) = maximum over j≥0 of
 *          S_j =
 *          Σ_{i=1..j} (-1)^{i+j} * q_i ,   S_0 = 0
 *          where q is the list of prime indices of n
 *          (each prime index repeated multiplicity times, in increasing order).
 *
 *  Computation strategy:
 *  * Factorize n by trial division using a static prime table.
 *  * For each prime divisor p with exponent e:
 *      * get its index p_idx among all primes (1‑based)
 *      * apply the recurrence   S = p_idx – S   e times
 *  * keep the maximum value of S over all steps.
 *
 *  Complexity: O(π(n)) per call, fast enough for n up to at least 10000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Prime table infrastructure – static and lazy built               */
/* ------------------------------------------------------------------ */
static uint64_t *primes   = NULL;   /* array of primes, 0‑based */
static size_t   primes_cnt = 0;    /* number of primes stored   */
static uint64_t primes_lim = 0;    /* maximum prime index (last prime) */

/* ensure that primes array contains all primes ≤ limit     */
static void ensure_primes(uint64_t limit)
{
    if (limit <= primes_lim) return;

    size_t sz = (size_t)limit + 1;          /* array of size limit+1 */
    bool *is_prime = (bool*)calloc(sz, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *new_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) { free(is_prime); exit(EXIT_FAILURE); }

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_primes[pos++] = i;

    free(is_prime);
    free(primes);
    primes   = new_primes;
    primes_cnt = cnt;
    primes_lim  = limit;
}

/* return 1‑based index of prime p (p is guaranteed prime) */
static size_t prime_index(uint64_t p)
{
    if (p > primes_lim) ensure_primes(p);
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p) lo = mid + 1;
        else                 hi = mid;
    }
    return lo + 1;   /* 1‑based */
}

/* ------------------------------------------------------------------ */
/*  OEIS sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;            /* a(1) = 0 */

    uint64_t temp = (uint64_t)n;

    /* Build prime list up to n – sufficient for all prime factors */
    ensure_primes(temp);

    int64_t S   = 0;                 /* current partial alternating sum */
    int64_t maxS = 0;                /* maximum observed value            */

    /* Factorize n using trial division over the prime list */
    size_t i = 0;
    while (i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t m = (seq_t)prime_index(p);   /* prime index, 1‑based */
            for (size_t k = 0; k < exp; ++k) {
                S = (int64_t)m - S;
                if (S > maxS) maxS = S;
            }
        }
        ++i;
    }

    /* Remaining prime factor > sqrt(n), if any */
    if (temp > 1) {
        seq_t m = (seq_t)prime_index(temp);
        S = (int64_t)m - S;
        if (S > maxS) maxS = S;
    }

    return (seq_t)maxS;
}
