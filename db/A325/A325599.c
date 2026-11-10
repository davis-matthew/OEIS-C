/*
 * A325599  Difference sequence of A325597.
 * https://oeis.org/A325599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A325597(n+1) - A325597(n); A325597 is built by search:
 *            a(n) = least number not 2a(m)+a(m-1) for any m < n
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9937 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t A325597_a(idx_t n)
{
    static seq_t *values      = NULL;
    static size_t values_cap  = 0;
    static size_t values_len  = 0;

    static bool *blocked      = NULL;
    static size_t blocked_cap = 0;

    static seq_t candidate = 1;

    if (!values) {
        values_cap = 4;
        values = mem_new(seq_t, values_cap + 1);
        if (!values) abort();
    }
    if (!blocked) {
        blocked_cap = 4;
        blocked = (bool *)calloc(blocked_cap + 1, sizeof(bool));
        if (!blocked) abort();
    }

    if (n > values_cap) {
        size_t new_cap = values_cap ? values_cap : 4;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, values, (new_cap + 1));
        if (!tmp) abort();
        values = tmp;
        values_cap = new_cap;
    }

    while (values_len < n) {
        while (true) {
            size_t ci = (size_t)candidate;
            if (ci > blocked_cap) break;
            if (!blocked[ci]) break;
            ++candidate;
        }

        ++values_len;
        values[values_len] = candidate;

        if (values_len >= 2) {
            seq_t forbid = 2 * values[values_len] + values[values_len - 1];
            size_t fi = (size_t)forbid;

            if (fi > blocked_cap) {
                size_t new_cap = blocked_cap ? blocked_cap : 4;
                while (new_cap < fi) new_cap <<= 1;
                bool *tmp = (bool *)realloc(blocked, (new_cap + 1) * sizeof(bool));
                if (!tmp) abort();
                memset(tmp + blocked_cap + 1, 0, (new_cap - blocked_cap) * sizeof(bool));
                blocked = tmp;
                blocked_cap = new_cap;
            }

            blocked[fi] = true;
        }

        ++candidate;
    }

    return values[n];
}

seq_t a(idx_t n)
{
    seq_t hi = A325597_a(n + 1);
    seq_t lo = A325597_a(n);
    return hi - lo;
}
