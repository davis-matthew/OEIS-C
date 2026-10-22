/*
 * A345680  Nonnegative integers whose trajectory under iteration of taking
 *          the absolute value of the alternating sum of the squares of the
 *          digits (A257588) includes zero.
 * https://oeis.org/A345680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth nonnegative integer whose trajectory under the map
 *            f(x) = |Σ_i (-1)^i * d_i^2| (digits d_i of x in decimal, from
 *            most significant) contains 0
 * Range:     u8 n <= 55, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* ---------- local utility ------------------------------------------------ */
#define MAX_F 2000  /* all values produced by f fit in this range */

/* compute f(x) for unsigned 64‑bit x */
static uint64_t compute_f(uint64_t x)
{
    if (x == 0)
        return 0;

    int digits[20];
    int dcnt = 0;
    while (x > 0)
    {
        digits[dcnt++] = (int)(x % 10);
        x /= 10;
    }

    int64_t sum = 0;
    int sign = 1;
    for (int i = dcnt - 1; i >= 0; --i)
    {
        int sq = digits[i] * digits[i];
        sum += sign * sq;
        sign = -sign;
    }
    if (sum < 0)
        sum = -sum;
    return (uint64_t)sum;
}

/* dynamic array grow helper (does not use external utils) */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    size_t newcap = (*cap > 0) ? (*cap * 2) : 8;
    if (newcap < need)
        newcap = need;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* ---------- memoized check for leading to 0 -------------------------------- */
static unsigned char memo[MAX_F + 1];  /* 255 = unknown, 0 = no, 1 = yes */

static int leads_to_zero(uint64_t x)
{
    if (x == 0)
        return 1;

    if (x > MAX_F)
        return leads_to_zero(compute_f(x));

    unsigned char val = memo[(size_t)x];
    if (val != (unsigned char)255)
        return val;

    uint64_t cur = x;
    unsigned int path[128];
    int plen = 0;
    unsigned char result = 0;

    while (1)
    {
        if (cur == 0)
        {
            result = 1;
            break;
        }
        if (cur <= MAX_F && memo[(size_t)cur] != (unsigned char)255)
        {
            result = memo[(size_t)cur];
            break;
        }
        /* cycle detection */
        int j;
        for (j = 0; j < plen; ++j)
            if (path[j] == cur)
                break;
        if (j < plen)
        {
            result = 0;
            break;
        }
        path[plen++] = (unsigned int)cur;
        cur = compute_f(cur);
    }

    for (int i = 0; i < plen; ++i)
        memo[path[i]] = result;
    memo[(size_t)x] = result;

    return result;
}

/* ---------- main sequence function --------------------------------------- */
static seq_t  *seq      = NULL;
static size_t seqcap    = 0;
static size_t seqlen    = 0;
static idx_t   next_cand = 0;   /* next candidate to test */

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized)
    {
        memset(memo, 0xFF, sizeof(memo));   /* set all to unknown (255) */
        initialized = true;
    }

    /* extend sequence until we have at least n terms */
    while (seqlen < (size_t)n)
    {
        if (leads_to_zero((uint64_t)next_cand))
        {
            grow_seq(&seq, &seqcap, seqlen + 1);
            seq[seqlen++] = (seq_t)next_cand;
        }
        ++next_cand;
    }

    return seq[(size_t)(n - 1)];
}
