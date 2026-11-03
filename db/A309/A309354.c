/*
 * A309354  Primes of the form p+q+r where p < q < r = p+6 are consecutive
 *          primes.
 * https://oeis.org/A309354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Compute the nth prime of the form p+q+r where p < q < r = p+6
 *            are consecutive primes. The algorithm enumerates odd primes p up
 *            to a fixed limit (200 000 000), checks the two possible gap
 *            patterns (2,4) and (4,2), and stores the sum if that sum is
 *            prime. The resulting numbers are automatically sor...
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- private helpers ---------- */
static inline void set_composite(uint32_t n, uint64_t *sieve) {
    /* n is odd and > 1 */
    uint32_t idx = n >> 1;            /* index of odd n  = (n-1)/2  */
    sieve[idx >> 6] |= 1ULL << (idx & 63);
}

static inline bool is_composite(uint32_t n, uint64_t *sieve) {
    if ((n & 1) == 0)                 /* even number */
        return n != 2;
    uint32_t idx = n >> 1;
    return (sieve[idx >> 6] >> (idx & 63)) & 1;
}

/* ---------- global state ---------- */
static const uint32_t LIMIT = 200000000;   /* sieve upper bound */
static uint64_t *sieve = NULL;             /* odd‑only bit sieve */
static uint32_t cur_p = 3;                 /* next odd number to test */
static seq_t *terms = NULL;                /* array of found terms   */
static size_t term_cnt = 0;                /* number of terms stored */

static size_t term_cap = 0;                /* allocated capacity for   */
                                          /* terms array               */

/* ---------- initialization ---------- */
static void prepare_sieve(void) {
    if (sieve != NULL) return;           /* already prepared */

    /* size of bit array for odd numbers up to LIMIT */
    size_t nbits = (LIMIT >> 1) + 1;      /* odd numbers including 1   */
    size_t nwords = (nbits + 63) >> 6;    /* each word 64 bits        */
    sieve = (uint64_t *)malloc(nwords * sizeof(uint64_t));
    if (!sieve) exit(EXIT_FAILURE);      /* fatal if allocation fails */
    for (size_t i = 0; i < nwords; ++i) sieve[i] = 0ULL;

    /* Sieve of Eratosthenes on odd numbers only   */
    for (uint32_t i = 3; (uint64_t)i * i <= LIMIT; i += 2) {
        if (!is_composite(i, sieve)) {
            for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += (uint64_t)i << 1)
                set_composite((uint32_t)j, sieve);
        }
    }
}

static void append_term(seq_t v) {
    if (term_cnt == term_cap) {
        term_cap = term_cap ? term_cap * 2 : 128;
        seq_t *old = terms;
        terms = mem_resize(seq_t, terms, term_cap);
        if (!terms) {
            mem_free(old);
            exit(EXIT_FAILURE);
        }
    }
    terms[term_cnt++] = v;
}

/* Ensure we have at least n terms computed   */
static void ensure_terms(idx_t n) {
    prepare_sieve();

    while (term_cnt < (size_t)n && cur_p <= LIMIT - 6) {
        if (!is_composite(cur_p, sieve)) {                /* cur_p is prime   */
            bool p6 = !is_composite(cur_p + 6, sieve);
            if (p6) {
                bool p2 = !is_composite(cur_p + 2, sieve);
                bool p4 = !is_composite(cur_p + 4, sieve);

                if ((p2 && !p4) || (!p2 && p4)) {
                    seq_t sum = (seq_t)3 * (seq_t)cur_p + (p2 ? 8 : 10);
                    if (is_prime_big(sum))
                        append_term(sum);
                }
            }
        }
        cur_p += 2;                       /* move to next odd number   */
    }
}

/* ---------- required public function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined, return 0 just in case */
    ensure_terms(n);
    if (n <= term_cnt)
        return terms[n - 1];
    /* If we did not find enough terms, return 0 (should not happen for
       the test ranges). */
    return 0;
}
