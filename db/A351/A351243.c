/*
 * A351243  Counterexamples to a conjecture of Selfridge and Lacampagne.
 * https://oeis.org/A351243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   search. k (not divisible by 3) is in the sequence iff no j
 *            exists with j and j*k both in A147991 (balanced-ternary digits
 *            all +-1). Decided by BFS over carry states of a digit automaton;
 *            a(n) is the n-th such k
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A351243_inS(long long c)
{
    if (c <= 0) return false;
    while (c > 0) {
        long long r = c % 3;
        if (r == 0) return false;
        if (r == 1) c = (c - 1) / 3;
        else c = (c + 1) / 3;
    }
    return true;
}

static uint32_t *A351243_stamp = NULL;
static long long *A351243_queue = NULL;
static size_t A351243_cap = 0;
static uint32_t A351243_cur = 0;

static bool A351243_is_quotient(long long k)
{
    size_t need = (size_t)(2 * k + 5);
    if (need > A351243_cap) {
        free(A351243_stamp);
        free(A351243_queue);
        A351243_stamp = (uint32_t *)calloc(need, sizeof(uint32_t));
        A351243_queue = (long long *)malloc(need * sizeof(long long));
        A351243_cap = need;
        A351243_cur = 0;
        if (!A351243_stamp || !A351243_queue) { A351243_cap = 0; return true; }
    }
    A351243_cur++;
    size_t head = 0, tail = 0;
    long long off = k + 2;
    A351243_queue[tail++] = 0;
    A351243_stamp[(size_t)off] = A351243_cur;
    while (head < tail) {
        long long c = A351243_queue[head++];
        for (int d = -1; d <= 1; d += 2) {
            long long v = (long long)d * k + c;
            long long r = ((v % 3) + 3) % 3;
            if (r == 0) continue;
            if (r == 2) r = -1;
            long long c2 = (v - r) / 3;
            if (d == 1 && (c2 == 0 || A351243_inS(c2))) return true;
            long long idx = c2 + off;
            if (idx < 0 || (size_t)idx >= A351243_cap) continue;
            if (A351243_stamp[(size_t)idx] != A351243_cur) {
                A351243_stamp[(size_t)idx] = A351243_cur;
                A351243_queue[tail++] = c2;
            }
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    static long long *found = NULL;
    static size_t nfound = 0, fcap = 0;
    static long long next_k = 1;

    if (n == 0) return (seq_t)0;
    while (nfound < (size_t)n) {
        long long k = next_k++;
        if (k % 3 == 0) continue;
        if (!A351243_is_quotient(k)) {
            if (nfound == fcap) {
                size_t nc = fcap ? fcap * 2 : 64;
                long long *t = (long long *)realloc(found, nc * sizeof(long long));
                if (!t) return (seq_t)0;
                found = t;
                fcap = nc;
            }
            found[nfound++] = k;
        }
    }
    return (seq_t)found[(size_t)n - 1];
}
