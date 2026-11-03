/*
 * A282903  Concatenation of elements of P{d_1}, P{d_2}, P{d_3}, ..., P{d_n};
 *          where P{d_n} denote the power set of the set of divisors of n
 *          ordered by the size of the subsets, and in each subset, following
 *          the increasing order.
 * https://oeis.org/A282903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th digit of OEIS A282903.
   The sequence is obtained by writing, for each integer k = 1, 2, 3, …,
   the decimal digits of the numbers produced by concatenating the elements
   of every non‑empty subset of the divisors of k, subsets being ordered
   first by increasing size and then lexicographically.  The function
   returns the value of the digit at position n (1‑based). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----------------------------------------------------------------------
   Generate all divisors of n recursively.
   ---------------------------------------------------------------------- */
static void _gen_divs(uint64_t cur, size_t idx,
                      const uint64_t *pr, const uint64_t *ex,
                      size_t pcnt, uint64_t *divs, size_t *cnt)
{
    if (idx == pcnt) {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        _gen_divs(val, idx + 1, pr, ex, pcnt, divs, cnt);
        val *= p;
    }
}

/* ----------------------------------------------------------------------
   Process a subset of size s and update the digit counter.
   ---------------------------------------------------------------------- */
static void process_divisor_digits(uint64_t val,
                                   idx_t *count, idx_t target,
                                   seq_t *result, bool *found)
{
    char buf[32];
    int len = 0;
    do {
        buf[len++] = (char)('0' + (val % 10));
        val /= 10;
    } while (val);

    for (int k = len - 1; k >= 0 && !(*found); --k) {
        (*count)++;
        if ((*count) == target) {
            *result = (seq_t)(buf[k] - '0');
            *found  = true;
        }
    }
}

static void process_size(const uint64_t *divs, size_t d,
                         size_t s,
                         idx_t target, idx_t *count,
                         seq_t *result, bool *found)
{
    if (s > d) return;

    /* static array big enough for our intended use (divisor counts are small) */
    size_t comb[256];
    size_t i;
    for (i = 0; i < s; ++i) comb[i] = i;

    for (;;) {
        /* iterate over the current subset */
        for (i = 0; i < s && !(*found); ++i) {
            process_divisor_digits(divs[comb[i]], count, target, result, found);
        }
        if (*found) return;

        /* next combination */
        int pos = (int)s - 1;
        while (pos >= 0 && comb[pos] == d - s + pos) pos--;
        if (pos < 0) break;
        comb[pos]++;
        for (int j = pos + 1; j < (int)s; ++j) comb[j] = comb[j - 1] + 1;
    }
}

/* ----------------------------------------------------------------------
   Compare function for qsort (ascending uint64_t)
   ---------------------------------------------------------------------- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t ia = *(const uint64_t *)a;
    uint64_t ib = *(const uint64_t *)b;
    return (ia < ib) ? -1 : (ia > ib) ? 1 : 0;
}

static void get_divisors(uint64_t n, uint64_t *divs, size_t *cnt)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    uint64_t tmp = n;

    /* factorisation (trial division) */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    *cnt = 0;
    _gen_divs(1, 0, primes, exps, pcnt, divs, cnt);
    qsort(divs, *cnt, sizeof(uint64_t), cmp_uint64);
}

/* ----------------------------------------------------------------------
   Main function: a(n)
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t      count  = 0;
    seq_t      result = 0;
    bool       found  = false;

    for (uint64_t k = 1; !found; ++k) {
        uint64_t divs[256];
        size_t   dcnt = 0;
        get_divisors(k, divs, &dcnt);

        for (size_t s = 1; s <= dcnt && !found; ++s) {
            process_size(divs, dcnt, s, n, &count, &result, &found);
        }
    }
    return result;
}
