/*
 * A392609  Number of prime factors of n that do not divide A276086(n), where
 *          A276086 is the primorial base exp-function.
 * https://oeis.org/A392609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Count of distinct prime divisors p of n such that p does
 *            NOT divide A276086(n) (the primorial base exp‑function)
 * Range:     u8 n <= 8162, u16 n <= 8162, u32 n <= 8162, u64 n <= 8162,
 *            u128 n <= 8162, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- helper to generate a small list of primes ---------- */
#define PRIME_LIMIT 2000 /* sufficient for n up to at least 10⁴ */
static bool sieve[PRIME_LIMIT];
static uint64_t primes[PRIME_LIMIT];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0) return;          /* already initialized */
    for (size_t i = 0; i < PRIME_LIMIT; ++i) sieve[i] = false;
    for (size_t i = 2; i < PRIME_LIMIT; ++i)
    {
        if (!sieve[i])
        {
            primes[prime_cnt++] = i;
            if (i * i < PRIME_LIMIT)
                for (size_t j = i * i; j < PRIME_LIMIT; j += i)
                    sieve[j] = true;
        }
    }
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* safety for unexpected input */

    init_primes();

    /* ---- distinct prime factors of n ---- */
    uint64_t distinct[64];
    size_t dcnt = 0;
    uint64_t temp = (uint64_t)n;

    for (size_t i = 0; i < prime_cnt &&
                        primes[i] <= temp / primes[i]; ++i)
    {
        uint64_t p = primes[i];
        if (temp % p == 0)
        {
            distinct[dcnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1)          /* remaining prime factor */
        distinct[dcnt++] = temp;

    /* ---- primorial base digits of n ---- */
    idx_t tmp = n;
    size_t digits_len = 0;
    uint8_t digits[128];      /* plenty of space for our limits */

    for (size_t i = 0; i < prime_cnt && tmp > 0; ++i)
    {
        uint64_t p = primes[i];
        digits[digits_len++] = (uint8_t)(tmp % p);
        tmp /= p;
        if (digits_len >= sizeof(digits)) break; /* safety */
    }

    /* ---- count primes of n that do not divide A276086(n) ---- */
    size_t not_divides = 0;
    for (size_t f = 0; f < dcnt; ++f)
    {
        uint64_t p = distinct[f];
        bool divides = false;
        for (size_t i = 0; i < digits_len; ++i)
        {
            if (primes[i] == p && digits[i] != 0)
            {
                divides = true;
                break;
            }
        }
        if (!divides) ++not_divides;
    }

    return (seq_t)not_divides;
}
