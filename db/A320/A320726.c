/*
 * A320726  Composite numbers such that all other numbers obtained from all
 *          permutations of all subsets of the digits are noncomposite.
 * https://oeis.org/A320726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   digits_of, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A320726: Composite numbers whose all proper sub‑digit permutations are non‑composite.
 * The function a(idx_t n) returns the n‑th term (1‑based index) of the sequence.
 * It is computed on demand and cached for subsequent calls.
 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/digits_of.h"

/* Recursive helper: check all permutations of a selected set of digits.
 * Returns true if ALL generated numbers are non‑composite; false if a composite
 * number is encountered. Numbers equal to the target 'n' are ignored. */
static bool perm_check(const uint8_t *sel, size_t k, size_t pos,
                       bool *used, uint8_t *perm, seq_t target)
{
    if (pos == k) {
        seq_t val = 0;
        for (size_t t = 0; t < k; ++t) {
            val = val * 10 + perm[t];
        }
        if (val == target)            /* skip the number itself */
            return true;
        /* '0' and '1' are not composite */
        if (val > 1 && !is_prime_big(val))
            return false;            /* composite found */
        return true;
    }
    for (size_t i = 0; i < k; ++i) {
        if (used[i]) continue;
        used[i] = true;
        perm[pos] = sel[i];
        if (!perm_check(sel, k, pos + 1, used, perm, target)) {
            used[i] = false;
            return false;
        }
        used[i] = false;
    }
    return true;
}

/* Test whether a given composite number satisfies the sequence property. */
static bool is_candidate(seq_t n)
{
    if (n <= 3) return false;   /* 2 and 3 are prime */

    /* Digit extraction (most significant first) */
    idx_t num = (idx_t)n;
    int len = digits_of(num);
    if (len == 0) len = 1;
    uint8_t digits[20];
    idx_t temp = num;
    for (int pos = len - 1; pos >= 0; --pos) {
        digits[pos] = temp % 10;
        temp /= 10;
    }

    /* Enumerate all non‑empty subsets of digits */
    uint32_t fullmask = (1u << len) - 1u;
    for (uint32_t mask = 1; mask <= fullmask; ++mask) {
        /* Build selected digits for this subset */
        uint8_t sel[20];
        size_t selcnt = 0;
        for (int pos = 0; pos < len; ++pos) {
            if (mask & (1u << pos))
                sel[selcnt++] = digits[pos];
        }

        bool used[20] = {0};
        uint8_t perm[20];
        if (!perm_check(sel, selcnt, 0, used, perm, n))
            return false;        /* a composite number found */
    }
    return true;                  /* all checks passed */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Cache for already computed terms (sequence is known to have only 16 terms) */
    static seq_t cache[20];
    static size_t cache_size = 0;
    static idx_t next_candidate = 4;   /* start searching from 4 */

    /* Fill cache until it contains the nth term */
    while (cache_size < (size_t)n) {
        while (true) {
            seq_t cand = (seq_t)next_candidate;
            ++next_candidate; /* advance search pointer */

            if (!is_prime_big(cand) && cand > 1 && is_candidate(cand)) {
                if (cache_size < sizeof(cache)/sizeof(cache[0]))
                    cache[cache_size++] = cand;
                break; /* found next term */
            }
        }
    }
    return cache[n - 1];
}
