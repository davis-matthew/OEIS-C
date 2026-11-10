/*
 * A260743  Sequence A261220 shown in factorial base: a(n) =
 *          A007623(A261220(n)).
 * https://oeis.org/A260743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007623(A261220(n)); A261220 is found by a search over
 *            integers, testing each one's factorial-base digits
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

#define A260743_MAXD 64

static bool A261220_is_valid(seq_t val)
{
    seq_t remaining = val;
    idx_t pos = 1;
    idx_t cnt = 0;
    idx_t pos_array[A260743_MAXD];
    seq_t digit_array[A260743_MAXD];

    while (remaining > 0) {
        seq_t base = (seq_t)(pos + 1);
        seq_t d = remaining % base;
        if (d != 0) {
            pos_array[cnt] = pos;
            digit_array[cnt] = d;
            cnt++;
        }
        remaining /= base;
        pos++;
    }
    for (idx_t x = 0; x < cnt; ++x) {
        idx_t i = pos_array[x];
        idx_t left_i = i - (idx_t)digit_array[x];
        for (idx_t y = x + 1; y < cnt; ++y) {
            idx_t j = pos_array[y];
            idx_t left_j = j - (idx_t)digit_array[y];
            if (left_i == j || left_j == i || left_i == left_j)
                return false;
        }
    }
    return true;
}

static seq_t *A261220_cache = NULL;
static size_t A261220_cap = 0;
static size_t A261220_len = 0;
static seq_t A261220_next = 0;

static void A261220_enlarge(size_t need)
{
    if (A261220_cap >= need) return;
    size_t new_cap = A261220_cap * 2;
    if (new_cap < need) new_cap = need;
    if (new_cap < 16) new_cap = 16;
    seq_t *arr = mem_new(seq_t, new_cap);
    if (!arr) exit(EXIT_FAILURE);
    for (size_t i = 0; i < A261220_len; ++i) arr[i] = A261220_cache[i];
    mem_free(A261220_cache);
    A261220_cache = arr;
    A261220_cap = new_cap;
}

static seq_t A261220_a(idx_t n)
{
    while (A261220_len <= (size_t)n) {
        if (A261220_is_valid(A261220_next)) {
            A261220_enlarge(A261220_len + 1);
            A261220_cache[A261220_len] = A261220_next;
            A261220_len++;
        }
        A261220_next = A261220_next + 1;
    }
    return A261220_cache[(size_t)n];
}

static seq_t A007623_a(seq_t m)
{
    if (m == 0) return 0;
    seq_t result = 0;
    seq_t q = m;
    seq_t i = 2;
    seq_t digits[A260743_MAXD];
    int nd = 0;
    while (q > 0) {
        digits[nd++] = q % i;
        q = q / i;
        i++;
    }
    for (int j = nd; j > 0; j--) {
        result = result * 10 + digits[j - 1];
    }
    return result;
}

seq_t a(idx_t n)
{
    return A007623_a(A261220_a(n));
}
