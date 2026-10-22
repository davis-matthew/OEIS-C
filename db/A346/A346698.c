/*
 * A346698  Sum of the even-indexed parts (even bisection) of the multiset of
 *          prime indices of n.
 * https://oeis.org/A346698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the sum of the even‑indexed parts of the multiset of *
 *            prime indices of n. The implementation factorises n, builds the
 *            list * of prime indices with multiplicities, sorts it implicitly
 *            by working * with the primes in increasing order, and adds the
 *            indices that * occupy even positions (1‑ba...
 * Range:     u8 n <= 3237, u16 n <= 9972, u32 n <= 9972, u64 n <= 9972,
 *            u128 n <= 9972, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for the prime tables */
static uint32_t *primes      = NULL;   /* list of primes in order (1‑based index = position+1) */
static size_t   primes_cnt   = 0;      /* number of primes stored */
static idx_t    primes_limit = 0;      /* maximum prime value stored in the table */

static uint16_t *prime_index = NULL;   /* prime_index[p] = 1‑based index of prime p (0 if composite) */

/* build or extend a simple sieve of Eratosthenes up to 'limit' */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= primes_limit)
        return;                     /* already sufficient */

    /* new sieve */
    unsigned char *is_prime = (unsigned char*)calloc((size_t)limit + 1, 1);
    if (!is_prime) exit(1);

    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = 1;            /* mark all as potential primes */

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    /* allocate new tables */
    uint32_t *new_primes  = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    uint16_t *new_index   = (uint16_t*)calloc((size_t)limit + 1, sizeof(uint16_t)); /* zeroed */
    if (!new_primes || !new_index) exit(1);

    /* fill tables */
    size_t i = 0;
    for (idx_t v = 2; v <= limit; ++v)
        if (is_prime[v]) {
            new_primes[i] = (uint32_t)v;
            new_index[v]  = (uint16_t)(i + 1);   /* 1‑based index */
            ++i;
        }

    /* swap old tables */
    free(primes);
    free(prime_index);
    primes = new_primes;
    prime_index = new_index;
    primes_cnt = cnt;
    primes_limit = limit;

    free(is_prime);
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but return 0 */

    /* ensure prime data up to n */
    ensure_primes_up_to(n);

    /* small tables for distinct prime factors of n */
    seq_t indices[16];
    seq_t exps[16];
    size_t fc = 0;

    idx_t tmp = n;
    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
        idx_t p = (idx_t)primes[i];
        if (tmp % p == 0) {
            seq_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            indices[fc] = (seq_t)prime_index[p];
            exps[fc]    = e;
            ++fc;
        }
    }
    if (tmp > 1) {
        /* remaining prime factor */
        indices[fc] = (seq_t)prime_index[(uint32_t)tmp];
        exps[fc]    = (seq_t)1;
        ++fc;
    }

    /* sum the indices that fall on even positions in the sorted multiset */
    seq_t sum = 0;
    idx_t pos = 1;        /* current position in the multiset (1‑based) */

    for (size_t i = 0; i < fc; ++i) {
        idx_t c = (idx_t)exps[i];
        bool start_even = ((pos & 1) == 0);
        idx_t evens = (c + (start_even ? 1 : 0)) / 2;
        sum += indices[i] * (seq_t)evens;
        pos += c;
    }

    return sum;
}
