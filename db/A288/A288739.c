/*
 * A288739  Number of permutations without leading zeros of digits of n that
 *          have the same 2-adic valuation as n.
 * https://oeis.org/A288739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 405, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of permutations without leading zeros of the digits of n
 * that have the same 2‑adic valuation (exponent of 2 in the prime factorization) as n.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_digits.h"

static int v2(seq_t x)
{
    int cnt = 0;
    /* x is guaranteed to be > 0 for the input sequence */
    while ((x & 1) == 0 && x != 0) {
        x >>= 1;
        cnt++;
    }
    return cnt;
}

/* lexicographic next permutation for an array of uint8_t */
static int next_permutation(uint8_t *a, size_t n)
{
    if (n < 2) return 0;
    int i = (int)n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return 0;                /* last permutation reached */

    int j = (int)n - 1;
    while (a[j] <= a[i]) j--;

    uint8_t tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;

    /* reverse the tail */
    size_t l = (size_t)(i + 1);
    size_t r = n - 1;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        l++; r--;
    }
    return 1;
}

seq_t a(idx_t n)
{
    /* Extract digits of n into an array */
    uint8_t digits[30];
    size_t d;
    get_digits(n, digits, &d);

    /* Sort digits in ascending order to prepare for next_permutation */
    for (size_t i = 0; i < d - 1; ++i) {
        for (size_t j = 0; j < d - i - 1; ++j) {
            if (digits[j] > digits[j + 1]) {
                uint8_t tmp = digits[j];
                digits[j] = digits[j + 1];
                digits[j + 1] = tmp;
            }
        }
    }

    int v2_target = v2(n);
    seq_t count = 0;

    do {
        if (digits[0] == 0) continue;   /* skip permutations with leading zero */

        /* Build integer value from current permutation */
        seq_t val = 0;
        for (size_t i = 0; i < d; ++i) {
            val = val * 10 + digits[i];
        }

        if (v2(val) == v2_target) {
            count++;
        }
    } while (next_permutation(digits, d));

    return count;
}
