/*
 * A351517  a(n) = Product_{p|n, p prime} p^(pi(p) mod 2).
 * https://oeis.org/A351517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p|n, p prime} p^(π(p) mod 2) (product of odd‑indexed
 *            prime divisors)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *prime_list = NULL;   /* ascending list of primes */
static size_t prime_count = 0;        /* number of stored primes */

/* Generate the next prime after the current last prime in prime_list. */
static void generate_next_prime(void)
{
    uint64_t candidate;
    if (prime_count == 0) {
        candidate = 2;
    } else {
        candidate = prime_list[prime_count - 1] + 1;
        if (candidate & 1) { /* already odd */
            /* keep candidate odd */
        } else {
            ++candidate; /* skip even numbers */
        }
    }

    for (;;) {
        int is_prime = 1;
        for (size_t j = 0; j < prime_count && prime_list[j] * prime_list[j] <= candidate; ++j) {
            if (candidate % prime_list[j] == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            uint64_t *tmp = (uint64_t *)realloc(prime_list, (prime_count + 1) * sizeof(uint64_t));
            if (!tmp) /* allocation failed – keep list unchanged */
                return;
            prime_list = tmp;
            prime_list[prime_count++] = candidate;
            return;
        }
        candidate += 2;   /* next odd candidate */
    }
}

/* Return the 0‑based index of prime p in prime_list (assumes p is present). */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else if (prime_list[mid] > p)
            hi = mid;
        else
            return mid;
    }
    return prime_count; /* should not happen for a valid prime */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;   /* behaviour for n < 1 is irrelevant */

    if (n == 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Factor m using trial division with the growing prime list. */
    uint64_t temp = m;
    for (size_t i = 0; ; ++i) {
        if (i >= prime_count)
            generate_next_prime();

        uint64_t p = prime_list[i];
        if (p > temp / p) /* p*p > temp */
            break;

        if (temp % p == 0) {
            /* Include p if its position (0‑based) is even → 1‑based odd. */
            if ((i & 1) == 0)
                result = result * (seq_t)p;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* Remaining factor > 1 is a prime larger than sqrt(m). */
    if (temp > 1) {
        /* Ensure that prime_list contains this prime. */
        while (prime_count == 0 || prime_list[prime_count - 1] < temp)
            generate_next_prime();

        size_t idx = prime_index(temp);
        if ((idx & 1) == 0)
            result = result * (seq_t)temp;
    }

    return result;
}
