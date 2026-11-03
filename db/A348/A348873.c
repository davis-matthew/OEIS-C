/*
 * A348873  After a(1) = 1, the sequence is always extended with the smallest
 *          divisor d (not yet present in the sequence) of the last term t. If
 *          d doesn't exist, we extend the sequence with 3*t + 1 and repeat.
 *          See the Comments section for more details.
 * https://oeis.org/A348873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1.
   For n > 1, let t = a(n‑1). Find the smallest divisor d of t that
   has not appeared earlier in the sequence. If such d exists,
   a(n) = d; otherwise a(n) = 3*t + 1. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
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

static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* Check whether a value has already appeared in the sequence. */
static bool present(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == val)
            return true;
    return false;
}

/* Recursively generate all divisors of t (excluding 1 and t) and keep the
   smallest divisor that has not yet appeared in the sequence. */
static void gen_divs(const uint64_t *pr, const uint64_t *ex,
                     size_t pfcnt, size_t idx, uint64_t curr, uint64_t t,
                     seq_t *best, bool *found)
{
    if (idx == pfcnt) {
        if (curr > 1 && curr < t) {
            seq_t d = curr;
            if (!present(d)) {
                if (!*found || d < *best) {
                    *best = d;
                    *found = true;
                }
            }
        }
        return;
    }
    uint64_t p = pr[idx];
    uint64_t v = curr;
    for (uint64_t i = 0; i <= ex[idx]; ++i) {
        gen_divs(pr, ex, pfcnt, idx + 1, v, t, best, found);
        v *= p;
    }
}

/* Main function returning the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* offset not used in the sequence */

    size_t target = (size_t)n;

    if (seq_len == 0) {   /* initialise with a(1) = 1 */
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    while (seq_len < target) {
        seq_t t = seq[seq_len - 1];
        seq_t nxt;

        if (t == (seq_t)1 || is_prime_big(t)) {
            nxt = t * (seq_t)3 + (seq_t)1;
        } else {
            uint64_t primes[64];
            uint64_t exps[64];
            size_t pfcnt = 0;

            factor_u64((uint64_t)t, primes, exps, &pfcnt);

            bool found = false;
            seq_t best = 0;
            gen_divs(primes, exps, pfcnt, 0, (uint64_t)1, (uint64_t)t, &best, &found);

            if (found)
                nxt = best;
            else
                nxt = t * (seq_t)3 + (seq_t)1;
        }

        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = mem_resize(seq_t, seq, seq_cap);
        }
        seq[seq_len++] = nxt;
    }

    return seq[target - 1];
}
