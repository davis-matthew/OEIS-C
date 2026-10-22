/*
 * A324752  Number of strict integer partitions of n not containing 1 or any
 *          prime indices of the parts.
 * https://oeis.org/A324752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Count of strict integer partitions of n that do not contain
 * 1 or any prime indices of the parts.  See OEIS A324752.
 */

#include <stdbool.h>
#include <stddef.h>

#define NMAX 10000
#define MAX_FACTORS 6   /* maximum distinct prime factors for n <= NMAX */

/* --------------------------------------------------------------------- */
/*  Global data structures (filled once).                                */
/* --------------------------------------------------------------------- */
static bool prime_flag[NMAX + 1];
static int  prime_idx[NMAX + 1];           /* rank of prime (1‑based) */
static int  spf[NMAX + 1];                /* smallest prime factor */
static int  forbid_idx[NMAX + 1][MAX_FACTORS];
static int  forbid_len[NMAX + 1];
static bool banned[NMAX + 1];
static bool prepared = false;

/* --------------------------------------------------------------------- */
/*  Pre‑compute primes, spf, prime indices and forbidden index lists.   */
/* --------------------------------------------------------------------- */
static void prepare(void)
{
    if (prepared) return;

    /* sieve for smallest prime factor and prime indices */
    for (int i = 0; i <= NMAX; ++i) spf[i] = 0;
    spf[0] = 0;  spf[1] = 1;
    int prime_counter = 0;
    for (int i = 2; i <= NMAX; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            prime_flag[i] = true;
            prime_counter++;
            prime_idx[i] = prime_counter;   /* 1‑based rank of prime */
            if ((long long)i * i <= NMAX) {
                for (int j = i * i; j <= NMAX; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        } else {
            prime_flag[i] = false;
        }
    }

    /* forbidden prime indices for each integer */
    for (int i = 2; i <= NMAX; ++i) {
        int x = i;
        int last = 0;
        int k = 0;
        while (x > 1) {
            int f = spf[x];
            if (f != last) {
                forbid_idx[i][k++] = prime_idx[f];  /* prime index of f */
                last = f;
            }
            while (x % f == 0) x /= f;
        }
        forbid_len[i] = k;
    }

    prepared = true;
}

/* --------------------------------------------------------------------- */
/*  Recursive search.  rem   – remaining sum to partition  
 *  maxPart – current maximum part allowed (strict decreasing).          */
/* --------------------------------------------------------------------- */
static seq_t dfs(int rem, int maxPart)
{
    if (rem == 0) return (seq_t)1;          /* successful partition */
    if (maxPart < 2) return (seq_t)0;       /* cannot use 1 or smaller */

    /* bound: sum of all numbers 2..maxPart */
    int maxSum = maxPart * (maxPart + 1) / 2 - 1;
    if (rem > maxSum) return (seq_t)0;      /* impossible with current max */

    seq_t total = 0;
    int start = (rem < maxPart) ? rem : maxPart;

    for (int part = start; part >= 2; --part) {
        if (banned[part]) continue;          /* part is prohibited */

        /* mark forbidden indices introduced by this part */
        int fcnt = forbid_len[part];
        bool added[MAX_FACTORS] = {0};
        for (int k = 0; k < fcnt; ++k) {
            int idx = forbid_idx[part][k];
            if (!banned[idx]) {
                banned[idx] = true;
                added[k] = true;
            }
        }

        total += dfs(rem - part, part - 1);

        /* restore banned marks */
        for (int k = 0; k < fcnt; ++k) {
            if (added[k]) {
                int idx = forbid_idx[part][k];
                banned[idx] = false;
            }
        }
    }
    return total;
}

/* --------------------------------------------------------------------- */
/*  Public interface.                                                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    prepare();

    if (n == 0) return (seq_t)1;
    if (n < 2)  return (seq_t)0;

    int N = (int)n;
    /* reset banned flags for values up to N */
    for (int i = 2; i <= N; ++i) banned[i] = false;
    banned[1] = true;        /* 1 is always forbidden */

    return dfs(N, N);
}
