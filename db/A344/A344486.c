/*
 * A344486  a(n) is the least k such that the sum of digits of k is a
 *          substring of n and the sum of digits of n is a substring of k.
 * https://oeis.org/A344486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest k such that sum_digits(k) is a substring of
 *            n and sum_digits(n) is a substring of k. The sequence satisfies
 *            a(10 * n) = a(n). This implementation searches all digit
 *            substrings of n and, for each, constructs the minimal k with d
 *            (the sum of digits of n) occurring either as a p...
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/seq_to_str.h"

static seq_t
build_candidate(const char *d_str, size_t len_d,
                unsigned int rem, bool prefix_first)
{
    seq_t val = 0;
    unsigned int total = rem;
    unsigned int digits = (rem + 8) / 9;   /* minimal number of digits */

    /* Build the digits that achieve the required sum */
    for (unsigned int i = 0; i < digits; ++i) {
        unsigned int remaining = digits - i - 1;
        unsigned int digit = (total > 9 * remaining) ? total - 9 * remaining : 0;
        if (prefix_first)
            /* prefix consists of the digits first, then d_str */
            val = val * 10 + (seq_t)digit;
        else
            /* suffix consists of the digits after d_str */
            val = val * 10 + (seq_t)digit;
        total -= digit;
    }

    if (!prefix_first) {
        /* d_str first, then the digits (suffix) */
        for (size_t i = 0; i < len_d; ++i)
            val = val * 10 + (seq_t)(d_str[i] - '0');
    } else {
        /* digits (prefix) first, then d_str */
        for (size_t i = 0; i < len_d; ++i)
            val = val * 10 + (seq_t)(d_str[i] - '0');
    }

    return val;
}

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;

    /* sum of digits of n and sum of digits of that sum */
    seq_t d_val = digit_sum(n_val);
    seq_t sd_val = digit_sum(d_val);
    unsigned int sum_d = (unsigned int)sd_val;

    /* Convert n and d to decimal strings */
    char n_str[32];
    size_t len_n;
    seq_to_str(n_val, n_str, &len_n);

    char d_str[32];
    size_t len_d;
    seq_to_str(d_val, d_str, &len_d);

    bool found = false;
    seq_t best = 0;

    /* Iterate over all substrings of n */
    for (size_t i = 0; i < len_n; ++i) {
        unsigned int val = 0;
        for (size_t j = i; j < len_n; ++j) {
            val = val * 10 + (n_str[j] - '0');
            if (val < sum_d)
                continue;                    /* cannot serve as sum_digits(k) */

            unsigned int rem = val - sum_d;

            /* Candidate 1: d as prefix, suffix gives remaining sum */
            seq_t cand1 = 0;
            {
                seq_t v = 0;
                /* d prefix */
                for (size_t k = 0; k < len_d; ++k)
                    v = v * 10 + (seq_t)(d_str[k] - '0');
                unsigned int total = rem;
                unsigned int dig = (rem + 8) / 9;
                for (unsigned int t = 0; t < dig; ++t) {
                    unsigned int remaining = dig - t - 1;
                    unsigned int digit = (total > 9 * remaining) ? total - 9 * remaining : 0;
                    v = v * 10 + (seq_t)digit;
                    total -= digit;
                }
                cand1 = v;
            }

            /* Candidate 2: d as suffix, prefix gives remaining sum */
            seq_t cand2 = 0;
            {
                seq_t v = 0;
                unsigned int total = rem;
                unsigned int dig = (rem + 8) / 9;
                for (unsigned int t = 0; t < dig; ++t) {
                    unsigned int remaining = dig - t - 1;
                    unsigned int digit = (total > 9 * remaining) ? total - 9 * remaining : 0;
                    v = v * 10 + (seq_t)digit;
                    total -= digit;
                }
                for (size_t k = 0; k < len_d; ++k)
                    v = v * 10 + (seq_t)(d_str[k] - '0');
                cand2 = v;
            }

            /* Update best */
            if (!found || cand1 < best) {
                best = cand1;
                found = true;
            }
            if (cand2 < best) {
                best = cand2;
                found = true;
            }
        }
    }

    return found ? best : (seq_t)0;
}
