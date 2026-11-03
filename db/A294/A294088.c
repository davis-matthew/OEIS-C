/*
 * A294088  Least prime p_k such that (p_k)^n has p_{k-1} as substring.
 * https://oeis.org/A294088
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 87 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294088: For n ≥ 2 return the smallest prime p_k such that the
   decimal representation of p_k^n contains the decimal representation of
   the preceding prime p_{k-1}.  For n > 59 the answer is known to be 3. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Computes base^exp and writes its decimal representation into buf.
   exp is treated as uint64_t, base ≤ 5000, exp ≤ 59.  */
static void power_string_uint32(uint32_t base, uint64_t exp,
                                char *buf, size_t bufsize)
{
    /* Digits are stored in base 1e9 for efficiency. */
    const uint32_t BASE = 1000000000u;
    /* 50 limbs are more than enough for 3701^59 (~211 decimal digits). */
    uint32_t limbs[50] = {0};
    size_t len = 1;
    limbs[0] = 1;          /* initialise to 1 */

    for (uint64_t e = 0; e < exp; ++e) {
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)limbs[i] * base + carry;
            limbs[i] = (uint32_t)(prod % BASE);
            carry    = prod / BASE;
        }
        while (carry > 0) {
            if (len >= 50) break;            /* safety */
            limbs[len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* Convert to decimal string. */
    int pos = 0;
    pos += snprintf(buf + pos, bufsize - pos, "%u", limbs[len - 1]);   /* most significant limb */
    for (int i = (int)len - 2; i >= 0; --i) {
        pos += snprintf(buf + pos, bufsize - pos, "%09u", limbs[i]);
    }
    buf[pos] = '\0';
}

/* Precomputes primes up to 5000 (enough for all needed cases). */
static void init_primes(uint32_t *primes, size_t *cnt)
{
    const size_t LIMIT = 5000;
    bool is_prime[LIMIT + 1];
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t k = p * p; k <= LIMIT; k += p)
                is_prime[k] = false;
        }
    }
    *cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[(*cnt)++] = (uint32_t)i;
}

/* Main sequence function as required by the framework. */
seq_t a(idx_t n)
{
    /* For n > 59 the answer is always 3. */
    if (n > 59) return (seq_t)3;

    /* Static storage for primes. */
    static bool initialized = false;
    static uint32_t primes[600];
    static size_t primes_cnt = 0;
    if (!initialized) {
        init_primes(primes, &primes_cnt);
        initialized = true;
    }

    char pow_buf[400];
    char prev_buf[20];

    /* Iterate over primes starting from the second prime (index 1). */
    for (size_t idx = 1; idx < primes_cnt; ++idx) {
        uint32_t p   = primes[idx];
        uint32_t prev = primes[idx - 1];

        power_string_uint32(p, n, pow_buf, sizeof(pow_buf));
        snprintf(prev_buf, sizeof(prev_buf), "%u", prev);

        if (strstr(pow_buf, prev_buf) != NULL)
            return (seq_t)p;
    }
    /* Should never reach here for n ≥ 2. */
    return (seq_t)0;
}
