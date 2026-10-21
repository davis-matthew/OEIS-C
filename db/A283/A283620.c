/*
 * A283620  a(n) is the least exponent k such that 3^k-1 is divisible by
 *          prime(n)^2, or -1 if no such k exists.
 * https://oeis.org/A283620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = the least exponent k such that 3^k ≡ 1 (mod prime(n)^2),
 *            or -1 if no such k exists (this happens only for prime(n)=3)
 * Range:     i8 n <= 6, i16 n <= 44, i32 n <= 4792, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Small sieve limit – enough primes to handle n up to at least 10 000. */
#define SIEVE_MAX 200000

static uint32_t primes[SIEVE_MAX];       /* list of primes */
static size_t primes_cnt = 0;            /* number of primes stored */
static uint8_t sieve_flag[SIEVE_MAX+1];  /* mask for sieve */

/* Build the prime table once, with numbers up to SIEVE_MAX. */
static void build_primes(void)
{
    if (primes_cnt) return;   /* already built */

    for (size_t i = 2; i <= SIEVE_MAX; ++i) sieve_flag[i] = 0;
    for (size_t i = 2; i <= SIEVE_MAX; ++i) {
        if (!sieve_flag[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= SIEVE_MAX) {
                for (size_t j = i * i; j <= SIEVE_MAX; j += i)
                    sieve_flag[j] = 1;
            }
        }
    }
}

/* Fast modular exponentiation for 64‑bit integers. */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 0) return 0;
    uint64_t res = 1 % mod;
    uint64_t b   = base % mod;
    while (exp) {
        if (exp & 1)
            res = (unsigned __int128)res * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return res;
}

/* Factorise n (<= SIEVE_MAX) into its distinct prime factors */
static void factorize_distinct(uint64_t n, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            /* Skip the exponent, we only need distinct primes. */
            while (n % p == 0) n /= p;
            factors[(*cnt)++] = p;
        }
    }
    if (n > 1) factors[(*cnt)++] = n;
}

/* The requested sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)-1;          /* out of range safety */

    /* Make sure we have the prime list built. */
    if (!primes_cnt) build_primes();

    /* Get the n‑th prime. */
    if ((idx_t)primes_cnt < n) return (seq_t)-1;   /* safety */
    uint32_t p = primes[n - 1];

    /* Handle the special case p = 3. */
    if (p == 3) return (seq_t)-1;

    /* Compute the order of 3 modulo p. */
    uint64_t r = (uint64_t)p - 1;
    uint64_t fac[20];
    size_t fac_cnt;
    factorize_distinct(r, fac, &fac_cnt);

    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t q = fac[i];
        while (r % q == 0) {
            uint64_t cand = r / q;
            if (mod_pow_u64(3, cand, p) == 1)
                r = cand;
            else
                break;
        }
    }

    /* Now decide whether the order modulo p² is r or r·p. */
    uint64_t modp2 = (uint64_t)p * (uint64_t)p;
    uint64_t val   = mod_pow_u64(3, r, modp2);
    uint64_t k     = (val == 1) ? r : r * (uint64_t)p;

    return (seq_t)k;
}
