/*
 * A386601  Primes that change into other primes of the same length at each of
 *          the four possible stages when all of their digits simultaneously
 *          shift along the ...8-9-0-1-2-3-4-5-6-7-8-9-0-1-2-3... cycle. No
 *          leading zeros and so no differing lengths of the changing primes
 *          are allowed.
 * https://oeis.org/A386601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big, shift_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A386601:  primes p for which, for all
   offset values s in {0,…,9} that do not make the leading digit zero,
   the number obtained by replacing each decimal digit d of p by
   (d + s) mod 10 is again a prime.  The sequence consists of
   the primes for which exactly four such shifts (including s = 0)
   yield primes.  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/shift_digits.h"

static unsigned leading_digit(seq_t n)
{
    while (n >= 10) n /= 10;
    return (unsigned)n;
}

/* returns true iff n has exactly four offsets s (including s=0)
   that keep the leading digit non–zero and for which the shifted
   number is a prime. */
static bool good_prime(seq_t n)
{
    unsigned top = leading_digit(n);
    int cnt = 0;
    for (unsigned s = 0; s < 10; ++s) {
        if ((top + s) % 10 == 0) continue;   /* would lead to zero */
        seq_t m = shift_digits(n, s);
        if (is_prime_big(m)) ++cnt;
    }
    return cnt == 4;
}

/* ---------- sieve and cache ---------- */
#define SIEVE_LIMIT 20000000   /* should contain enough primes for n≤10000 */

static char *sieve_comp = NULL;            /* 0 = prime, 1 = composite */
static uint64_t *prime_arr = NULL;
static size_t prime_cap = 0, prime_len = 0;
static size_t prime_pos = 0;               /* next prime to test */

static seq_t *seq_cache = NULL;
static size_t seq_cap = 0, seq_len = 0;

static void init_sieve(void)
{
    if (sieve_comp) return;           /* already done */

    sieve_comp = (char*)malloc(SIEVE_LIMIT + 1);
    memset(sieve_comp, 0, SIEVE_LIMIT + 1);
    sieve_comp[0] = sieve_comp[1] = 1;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve_comp[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve_comp[j] = 1;
        }
    }

    /* grow prime array while scanning */
    prime_cap = 100000;
    prime_arr = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    prime_len = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) if (!sieve_comp[i]) {
        if (prime_len == prime_cap) {
            prime_cap *= 2;
            prime_arr = (uint64_t*)realloc(prime_arr,
                                           prime_cap * sizeof(uint64_t));
        }
        prime_arr[prime_len++] = i;
    }
}

/* enlarge the result cache if necessary */
static void ensure_cache_cap(size_t need)
{
    if (seq_len >= need) return;
    while (seq_len < need) {
        /* bring the next prime from the sieve */
        if (prime_pos >= prime_len) break;   /* out of primes */

        seq_t cand = (seq_t)prime_arr[prime_pos++];
        if (good_prime(cand)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq_cache = (seq_t*)realloc(seq_cache,
                                             seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = cand;
        }
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1 */

    init_sieve();
    ensure_cache_cap((size_t)n);

    if (n - 1 >= seq_len) return 0;      /* insufficient data */
    return seq_cache[(size_t)n - 1];
}
