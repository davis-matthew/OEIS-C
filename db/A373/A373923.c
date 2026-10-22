/*
 * A373923  a(n) = Sum_{d|n} (-1)^pi(d).
 * https://oeis.org/A373923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^pi(d)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- helper: integer square root (floor) */
static idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0 || mid <= n / mid) {
            /* mid*mid <= n, try increasing mid */
            if (mid + 1 > n / (mid + 1))
                return mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return 0; /* never reached */
}

/* --- storage for known primes */
static uint64_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static size_t prime_cap  = 0;
static idx_t   prime_last_checked = 1;   /* last number that was tested */

/* --- add new primes up to 'limit' using trial division */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_last_checked)
        return;

    if (prime_cap == 0) {                 /* first allocation */
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list)
            return;                      /* allocation failed – should not happen */
    }

    for (idx_t cand = prime_last_checked + 1; cand <= limit; ++cand) {
        bool is_p = true;
        idx_t sq = int_sqrt(cand);
        for (size_t k = 0; k < prime_cnt && prime_list[k] <= sq; ++k) {
            if (cand % prime_list[k] == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp)
                    return;              /* allocation failed */
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = cand;
        }
    }
    prime_last_checked = limit;
}

/* --- primality test using the stored prime list  */
static bool is_prime_simple(idx_t x)
{
    if (x < 2)
        return false;
    idx_t sq = int_sqrt(x);
    for (size_t k = 0; k < prime_cnt && prime_list[k] <= sq; ++k) {
        if (x % prime_list[k] == 0)
            return false;
    }
    return true;
}

/* --- storage for prime counting function pi(·)  */
static seq_t *pi_cache = NULL;   /* pi_cache[i] = pi(i) */
static idx_t pi_cap = 0;          /* highest computed index */

/* --- extend pi_cache up to 'n' */
static void ensure_pi_cache(idx_t n)
{
    if (n <= pi_cap)
        return;

    /* ensure primes list is large enough for pi computation */
    ensure_primes_up_to(n);

    seq_t *new_pi = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_pi)
        return;                  /* allocation failed – should not happen */

    seq_t last_val = 0;
    if (pi_cap == 0) {
        new_pi[0] = 0;
    } else {
        for (idx_t i = 0; i <= pi_cap; ++i)
            new_pi[i] = pi_cache[i];
        last_val = pi_cache[pi_cap];
    }

    for (idx_t i = (pi_cap == 0 ? 1 : pi_cap + 1); i <= n; ++i) {
        last_val += is_prime_simple(i) ? (seq_t)1 : (seq_t)0;
        new_pi[i] = last_val;
    }

    free(pi_cache);
    pi_cache = new_pi;
    pi_cap   = n;
}

/* --- the required sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* undefined for n = 0, but harmless */

    /* make sure we can answer queries up to n */
    ensure_pi_cache(n);

    seq_t sum = 0;

    /* iterate over divisor pairs d and n/d */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t sign_d = ((pi_cache[d] & 1) ? (seq_t)-1 : (seq_t)1);
            sum += sign_d;

            idx_t q = n / d;
            if (q != d) {
                seq_t sign_q = ((pi_cache[q] & 1) ? (seq_t)-1 : (seq_t)1);
                sum += sign_q;
            }
        }
    }

    return sum;
}
