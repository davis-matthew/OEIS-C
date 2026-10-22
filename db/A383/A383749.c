/*
 * A383749  Positive numbers k whose decimal expansion does not contain the
 *          decimal expansion of any proper divisor of k.
 * https://oeis.org/A383749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1557, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, contains_sub, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search. For each integer k > 0 the function
   checks whether its decimal representation contains the representation
   of any proper divisor of k; if not, k is accepted as a member of the
   sequence.  The k-th accepted integer is returned. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"
#include "../utils/contains_sub.h"

/* Static storage for the list of primes used by the factoriser */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure at least `limit` primes are available */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    if (prime_list) free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Convert a seq_t value to a decimal string.
   The buffer must be at least 64 bytes.  The string is written in forward
   order and *len receives its length. */
static void
seq_to_str(seq_t n, char *buf, int *len)
{
    seq_t tmp = n;
    if (tmp == (seq_t)0) {
        buf[0] = '0';
        *len    = 1;
        return;
    }
    char rev[64];
    int pos = 0;
    while (tmp > (seq_t)0) {
        seq_t d = tmp % (seq_t)10;
        rev[pos++] = '0' + (int)d;
        tmp /= (seq_t)10;
    }
    for (int i = 0; i < pos; ++i) buf[i] = rev[pos - 1 - i];
    *len = pos;
}

/* Recursive routine to generate all divisors from the prime factorisation.
   `p`, `e` hold the prime bases and their exponents.  `pc` is the number
   of distinct primes.  `orig` is the original number; it is omitted from
   the output.  Divisors are stored in `out`, and the count is accumulated
   in *cnt. */
static void
gen_divisors_recursive(int idx, seq_t cur,
                       const uint64_t *p, const uint8_t *e,
                       int pc, seq_t orig,
                       seq_t *out, int *cnt)
{
    if (idx == pc) {
        if (cur != (seq_t)1 && cur != orig) {
            out[(*cnt)++] = cur;
        }
        return;
    }
    seq_t base = (seq_t)p[idx];
    seq_t power = (seq_t)1;
    for (int exp = 0; exp <= e[idx]; ++exp) {
        if (exp > 0) power *= base;
        gen_divisors_recursive(idx + 1, cur * power, p, e, pc, orig, out, cnt);
    }
}

/* Test whether a candidate k satisfies the sequence property. */
static bool
is_candidate(seq_t k)
{
    if (k == (seq_t)1) return true;          /* 1 has no proper divisors */

    char kstr[64];
    int klen;
    seq_to_str(k, kstr, &klen);

    /* If the decimal expansion contains a digit '1', it immediately fails
       because 1 is a proper divisor.  However, k == 1 is handled above. */
    for (int i = 0; i < klen; ++i) {
        if (kstr[i] == '1') return false;
    }

    /* Prepare prime list up to sqrt(k) */
    idx_t sqrt_k = (idx_t)floor_sqrt(k);
    ensure_primes(sqrt_k);

    /* Factorise k using the prime list */
    seq_t tmp = k;
    uint64_t pf[64];
    uint8_t pe[64];
    int pfcnt = 0;
    for (size_t i = 0; i < prime_count && tmp > (seq_t)1; ++i) {
        uint64_t p = prime_list[i];
        seq_t pseq = (seq_t)p;
        if (pseq * pseq > tmp) break;
        if (tmp % pseq == 0) {
            uint8_t e = 0;
            do {
                tmp /= pseq;
                ++e;
            } while (tmp % pseq == 0);
            pf[pfcnt] = p;
            pe[pfcnt] = e;
            ++pfcnt;
        }
    }
    if (tmp > (seq_t)1) {          /* remaining prime factor > sqrt(k) */
        pf[pfcnt]   = (uint64_t)tmp;
        pe[pfcnt]   = 1;
        ++pfcnt;
    }

    /* Generate all proper divisors (>1) */
    seq_t divs[512];
    int dcnt = 0;
    gen_divisors_recursive(0, (seq_t)1, pf, pe, pfcnt, k, divs, &dcnt);

    /* Check each divisor for substring containment */
    char dstr[64];
    int dlen;
    for (int i = 0; i < dcnt; ++i) {
        seq_to_str(divs[i], dstr, &dlen);
        if (contains_sub(kstr, klen, dstr, dlen)) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Special cases for small n – the sequence begins 1..9 */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    if (n == (idx_t)3) return (seq_t)3;
    if (n == (idx_t)4) return (seq_t)4;
    if (n == (idx_t)5) return (seq_t)5;
    if (n == (idx_t)6) return (seq_t)6;
    if (n == (idx_t)7) return (seq_t)7;
    if (n == (idx_t)8) return (seq_t)8;
    if (n == (idx_t)9) return (seq_t)9;

    idx_t count = 0;
    seq_t k     = (seq_t)1;
    for (;;) {
        if (is_candidate(k)) {
            ++count;
            if (count == n) return k;
        }
        ++k;
    }
}
