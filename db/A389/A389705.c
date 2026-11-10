/*
 * A389705  Indices of record high points in A389704.
 * https://oeis.org/A389705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = index of the n-th record in A389704 (the n-th new maximal
 *            length of a strictly decreasing bigomega run), found by a
 *            segmented bigomega sieve scan with cached state
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A389705_SEG (1u << 16)
#define A389705_PL  (1u << 16)
#define A389705_MAXREC 256

static uint32_t A389705_primes[7000];
static int A389705_np = 0;
static bool A389705_init = false;

static uint8_t A389705_cnt[A389705_SEG];
static uint64_t A389705_prod[A389705_SEG];

static uint64_t A389705_rec[A389705_MAXREC];
static int A389705_nrec = 0;
static uint64_t A389705_best = 0;
static uint64_t A389705_next = 1;   /* next index to process */
static uint64_t A389705_start = 1;
static uint64_t A389705_len = 0;
static int A389705_prev = 0;

static void A389705_setup(void)
{
    static uint8_t comp[A389705_PL];
    uint32_t i, j;
    for (i = 2; i < A389705_PL; i++)
    {
        if (!comp[i])
        {
            if (A389705_np < 7000)
                A389705_primes[A389705_np++] = i;
            for (j = i * i; j < A389705_PL; j += i)
                comp[j] = 1;
        }
    }
    A389705_init = true;
}

static void A389705_segment(void)
{
    uint64_t lo = A389705_next;
    uint64_t hi = lo + A389705_SEG;
    uint32_t i;
    int k;
    for (i = 0; i < A389705_SEG; i++)
    {
        A389705_cnt[i] = 0;
        A389705_prod[i] = 1;
    }
    for (k = 0; k < A389705_np; k++)
    {
        uint64_t p = A389705_primes[k];
        if (p * p >= hi)
            break;
        for (uint64_t q = p; q < hi; q *= p)
        {
            uint64_t m = ((lo + q - 1) / q) * q;
            for (; m < hi; m += q)
            {
                A389705_cnt[m - lo]++;
                A389705_prod[m - lo] *= p;
            }
        }
    }
    for (i = 0; i < A389705_SEG; i++)
    {
        uint64_t m = lo + i;
        int b = A389705_cnt[i];
        if (A389705_prod[i] != m)
            b++;
        if (A389705_len == 0)
        {
            A389705_start = m;
            A389705_len = 1;
            A389705_prev = b;
        }
        else if (b < A389705_prev)
        {
            A389705_len++;
            A389705_prev = b;
        }
        else
        {
            if (A389705_len > A389705_best && A389705_nrec < A389705_MAXREC)
            {
                A389705_best = A389705_len;
                A389705_rec[A389705_nrec++] = A389705_start;
            }
            A389705_start = m;
            A389705_len = 1;
            A389705_prev = b;
        }
    }
    A389705_next = hi;
}

seq_t a(idx_t n)
{
    if (!A389705_init)
        A389705_setup();
    while ((uint64_t)A389705_nrec < (uint64_t)n)
        A389705_segment();
    return (seq_t)A389705_rec[n - 1];
}
