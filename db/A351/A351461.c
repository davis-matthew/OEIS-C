/*
 * A351461  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A206787(i) = A206787(j) and A336651(i) = A336651(j) for all i,
 *          j >= 1.
 * https://oeis.org/A351461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of [A206787(n),
 *            A336651(n)], computed by scanning odd m <= odd part of n and
 *            numbering distinct pairs by first appearance
 * Range:     u8 n <= 216, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8618 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void A351461_keys(idx_t m, seq_t *s, seq_t *r)
{
    seq_t sum = 1, rest = 1;
    idx_t x = m;
    while ((x & 1) == 0) x >>= 1;
    for (idx_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            sum *= (seq_t)(p + 1);
            x /= p;
            while (x % p == 0) {
                x /= p;
                rest *= (seq_t)p;
            }
        }
    }
    if (x > 1) sum *= (seq_t)(x + 1);
    *s = sum;
    *r = rest;
}

seq_t a(idx_t n)
{
    idx_t o = n;
    while ((o & 1) == 0) o >>= 1;

    seq_t ts, tr;
    A351461_keys(o, &ts, &tr);

    size_t cap = (size_t)(o / 2 + 2);
    seq_t *ks = mem_new(seq_t, cap);
    seq_t *kr = mem_new(seq_t, cap);
    size_t cnt = 0;
    seq_t result = 0;

    for (idx_t m = 1; m <= o; m += 2) {
        seq_t s, r;
        A351461_keys(m, &s, &r);
        bool found = false;
        for (size_t i = 0; i < cnt; i++) {
            if (ks[i] == s && kr[i] == r) { found = true; break; }
        }
        if (!found) {
            ks[cnt] = s;
            kr[cnt] = r;
            cnt++;
            if (s == ts && r == tr) {
                result = (seq_t)cnt;
                break;
            }
        }
    }

    mem_free(ks);
    mem_free(kr);
    return result;
}
