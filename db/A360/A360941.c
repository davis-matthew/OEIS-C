/*
 * A360941  a(n) is the least multiple of m that is a happy number (A007770).
 * https://oeis.org/A360941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * k, where k is the smallest positive integer such that
 *            n*k is a happy number (A007770). The sequence offset is 1. This
 *            implementation searches successive multiples of n until it finds
 *            a happy number. It uses a small cache for numbers < 1000 to
 *            speed up the happiness test
 * Range:     u8 n <= 8, u16 n <= 899, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sumsq_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/sumsq_int.h"

/* ---------- Cache for happiness of numbers < 1000 ---------- */
static unsigned char happy_state[1000];
/* 0 = unknown, 1 = computing, 2 = happy, 3 = unhappy */
static char happy_inited = 0;

/* Recursively determine if a small number (< 1000) is happy. */
static bool compute_happy_small(int x)
{
    if (happy_state[x] == 2) return true;
    if (happy_state[x] == 3) return false;
    if (happy_state[x] == 1) {            /* cycle detected */
        happy_state[x] = 3;
        return false;
    }

    happy_state[x] = 1;                    /* mark as computing */
    int next = sumsq_int(x);
    bool res = compute_happy_small(next);
    happy_state[x] = res ? 2 : 3;
    return res;
}

/* Initialize the happiness cache once. */
static void init_happy(void)
{
    if (happy_inited) return;
    happy_inited = 1;
    happy_state[0] = 3;            /* 0 is not happy */
    happy_state[1] = 2;            /* 1 is happy */
    for (int i = 2; i < 1000; ++i) happy_state[i] = 0;
    for (int i = 2; i < 1000; ++i) (void)compute_happy_small(i);
}

/* Test if a number is happy (works for all seq_t types). */
static bool is_happy(seq_t v)
{
    init_happy();
    while (v >= 1000) {
        seq_t sum = 0;
        seq_t tmp = v;
        while (tmp > 0) {
            int d = (int)(tmp % (seq_t)10);
            sum += (seq_t)(d * d);
            tmp /= (seq_t)10;
        }
        v = sum;
    }
    return happy_state[(int)v] == 2;
}

/* ---------- Main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* safety for index 0 */
    seq_t sn = (seq_t)n;                   /* convert index first */
    seq_t candidate = sn;                  /* first multiple: n*1 */
    for (;;) {
        if (is_happy(candidate)) return candidate;

        /* Avoid unsigned wrap on 64‑bit builds. */
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            seq_t max_val = ~(seq_t)0;      /* maximal uint64_t value */
            if (candidate > max_val - sn)   /* next addition would wrap */
                return 0;                   /* sentinel (should not happen) */
        }
        candidate += sn;                   /* next multiple */
    }
}
