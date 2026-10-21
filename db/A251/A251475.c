/*
 * A251475  Number of (n+1)X(1+1) 0..3 arrays with every 2X2 subblock summing
 *          to a prime and those sums nondecreasing in every row and column.
 * https://oeis.org/A251475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays of 0..3 with each 2×2 sum prime
 *            and sums nondecreasing
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Prime values that can occur as sums of a 2×2 block (0..3 entries). */
static const int PRIMES[5] = { 2, 3, 5, 7, 11 };

/* Mapping from sum to its index in PRIMES, or -1 if not prime. */
static int primeIdx[13];
static bool _initialized = false;

/* Row values for the 16 possible 2‑tuples of entries (0..3). */
static int rowVal[16][2];
/* For each pair of rows the index of the prime sum (if any). */
static int pairPrimeIdx[16][16];
/* Flag whether the pair of rows yields a prime sum. */
static bool pairIsPrime[16][16];

/* One‑time precomputation. */
static void init_once(void) {
    if (_initialized)
        return;

    /* Build primeIdx table. */
    for (int i = 0; i < 13; i++)
        primeIdx[i] = -1;
    for (int i = 0; i < 5; i++)
        primeIdx[PRIMES[i]] = i;

    /* Build row values. */
    for (int r = 0; r < 16; r++) {
        rowVal[r][0] = r / 4;  /* first entry */
        rowVal[r][1] = r % 4;  /* second entry */
    }

    /* Build tables for pair sums. */
    for (int a = 0; a < 16; a++) {
        for (int b = 0; b < 16; b++) {
            int sum = rowVal[a][0] + rowVal[a][1] +
                      rowVal[b][0] + rowVal[b][1];
            if (sum <= 12) {
                int idx = primeIdx[sum];
                pairPrimeIdx[a][b] = idx;
                pairIsPrime[a][b] = idx != -1;
            } else {
                pairPrimeIdx[a][b] = -1;
                pairIsPrime[a][b] = false;
            }
        }
    }

    _initialized = true;
}

seq_t a(idx_t n) {
    init_once();

    if (n == 0)          /* offset is 1, but safe for n==0 */
        return 0;

    /* State tables for dynamic programming. */
    seq_t dpPrev[16][5] = {0};
    seq_t dpNext[16][5] = {0};

    /* First row can be any of the 16 2‑tuples. */
    seq_t ones[16];
    for (int r = 0; r < 16; r++)
        ones[r] = 1;

    /* Iterate through the n transitions (there are n subblocks). */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero dpNext. */
        for (int r = 0; r < 16; r++)
            for (int k = 0; k < 5; k++)
                dpNext[r][k] = 0;

        if (step == 1) {
            /* First transition: no previous sum constraint. */
            for (int rPrev = 0; rPrev < 16; rPrev++) {
                seq_t curCnt = ones[rPrev];
                if (curCnt == 0)
                    continue;
                for (int rNext = 0; rNext < 16; rNext++) {
                    if (!pairIsPrime[rPrev][rNext])
                        continue;
                    int pIdx = pairPrimeIdx[rPrev][rNext];
                    dpNext[rNext][pIdx] += curCnt;
                }
            }
        } else {
            /* Subsequent transitions: enforce nondecreasing sum. */
            for (int rPrev = 0; rPrev < 16; rPrev++) {
                for (int sPrev = 0; sPrev < 5; sPrev++) {
                    seq_t curCnt = dpPrev[rPrev][sPrev];
                    if (curCnt == 0)
                        continue;
                    int prevSum = PRIMES[sPrev];
                    for (int rNext = 0; rNext < 16; rNext++) {
                        if (!pairIsPrime[rPrev][rNext])
                            continue;
                        int pIdx = pairPrimeIdx[rPrev][rNext];
                        int sumNow = PRIMES[pIdx];
                        if (sumNow < prevSum)      /* nondecreasing? */
                            continue;
                        dpNext[rNext][pIdx] += curCnt;
                    }
                }
            }
        }

        /* Prepare for next step. */
        for (int r = 0; r < 16; r++)
            for (int k = 0; k < 5; k++)
                dpPrev[r][k] = dpNext[r][k];
    }

    /* Sum all counts after the last step. */
    seq_t total = 0;
    for (int r = 0; r < 16; r++)
        for (int k = 0; k < 5; k++)
            total += dpPrev[r][k];
    return total;
}
