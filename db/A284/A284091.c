/*
 * A284091  Indices n where prime(n) + 2*prime(n+1) and 2*prime(n) +
 *          prime(n+1) have the same number of prime divisors counted with
 *          multiplicity.
 * https://oeis.org/A284091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1019, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A284091: a(n) = the n‑th index k such that
   Ω(prime(k)+2*prime(k+1)) = Ω(2*prime(k)+prime(k+1))
   (Ω = number of prime factors counted with multiplicity). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  dynamic storage of generated primes  ---------- */
static uint64_t *primes   = NULL;  /* primes[0] = 2, primes[1] = 3, …   */
static size_t    prime_cap  = 0;
static size_t    prime_cnt  = 0;   /* number of primes stored */

/* ----------  dynamic storage of qualifying indices  ---------- */
static seq_t *seq      = NULL;    /* seq[0] = first qualifying index */
static size_t seq_cap  = 0;       /* allocated capacity of seq      */
static size_t seq_len  = 0;       /* number of elements stored      */

/* ----------  grow the primes array  ---------- */
static void grow_primes(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp)
        abort();             /* out of memory */
    primes = tmp;
    prime_cap = new_cap;
}

/* ----------  grow the seq array  ---------- */
static void grow_seq_array(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();             /* out of memory */
    seq      = tmp;
    seq_cap  = new_cap;
}

/* ----------  compute Ω(n) for a 64‑bit integer  ---------- */
static seq_t bigomega_uint64(uint64_t n)
{
    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n)
            break;
        while (n % p == 0) {
            n /= p;
            ++count;
        }
    }
    if (n > 1)
        ++count;          /* remaining prime factor */
    return (seq_t)count;
}

/* ----------  generate the next prime (trial division)  ---------- */
static void generate_next_prime(void)
{
    uint64_t candidate = (prime_cnt == 0) ? 2 : (primes[prime_cnt - 1] + 1);
    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            grow_primes(prime_cnt + 1);
            primes[prime_cnt++] = candidate;
            return;
        }
        ++candidate;
    }
}

/* ----------  ensure that at least 'target' qualifying indices are known  ---------- */
static void ensure_seq_size(idx_t target)
{
    if (target <= seq_len)
        return;
    /* we need room for 'target' indices */
    grow_seq_array(target);
    while (seq_len < (size_t)target) {
        generate_next_prime();               /* new prime added */
        if (prime_cnt < 2)                   /* need at least two primes for a pair */
            continue;
        /* current prime has index prime_cnt.
           The pair is prime_cnt-1 and prime_cnt (1‑based indices). */
        size_t idx = prime_cnt - 1;           /* 1‑based index of the first prime in the pair */
        uint64_t p1 = primes[idx - 1];        /* prime(idx)   */
        uint64_t p2 = primes[idx];            /* prime(idx+1) */
        seq_t a1 = (seq_t)p1 + ((seq_t)2) * p2;
        seq_t a2 = ((seq_t)2) * p1 + (seq_t)p2;
        seq_t o1 = bigomega_uint64((uint64_t)a1);
        seq_t o2 = bigomega_uint64((uint64_t)a2);
        if (o1 == o2)
            seq[seq_len++] = (seq_t)idx;     /* store qualifying index */
    }
}

/* ----------  public sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n < 1, safety return */
    ensure_seq_size(n);
    return seq[n - 1];
}
