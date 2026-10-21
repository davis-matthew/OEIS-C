/*
 * A294737  Numbers that are the sum of 5 nonzero squares in exactly 3 ways.
 * https://oeis.org/A294737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   numbers that are the sum of 5 nonzero squares in exactly 3
 *            unordered ways
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* The sequence is known to be finite with all terms below 50000. */
#define MAXN          60000
#define SQRT_MAXN      245        /* 245^2 = 60025 > MAXN */
#define MAX_TERMS    2000        /* enough for all terms */

/* Precomputed squares up to SQRT_MAXN */
static int squares[SQRT_MAXN + 1];

/* How many unordered representations each n has */
static uint16_t count_rep[MAXN + 1];

/* The sequence itself, 1‑based indexing.  terms_len holds the number of terms found. */
static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;

/* Floor of integer square root using the squares table. */
static int floor_isqrt(int v)
{
    int lo = 0, hi = SQRT_MAXN;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (squares[mid] <= v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Build the representation counts and collect the sequence terms. */
static void build_counts(void)
{
    /* Fill the squares table. */
    for (int i = 0; i <= SQRT_MAXN; ++i)
        squares[i] = i * i;

    int max_root = floor_isqrt(MAXN);

    for (int a = 1; a <= max_root; ++a) {
        int sa = squares[a];
        if (sa > MAXN) break;
        for (int b = a; b <= max_root; ++b) {
            int sb = squares[b];
            int sab = sa + sb;
            if (sab > MAXN) break;
            for (int c = b; c <= max_root; ++c) {
                int sc = squares[c];
                int sabc = sab + sc;
                if (sabc > MAXN) break;
                for (int d = c; d <= max_root; ++d) {
                    int sd = squares[d];
                    int sabcd = sabc + sd;
                    if (sabcd > MAXN) break;
                    int rem = MAXN - sabcd;
                    int max_e = floor_isqrt(rem);
                    for (int e = d; e <= max_e; ++e) {
                        int sum5 = sabcd + squares[e];
                        if (sum5 <= MAXN)
                            ++count_rep[sum5];
                    }
                }
            }
        }
    }

    /* Extract terms that have exactly 3 representations. */
    for (int n = 1; n <= MAXN; ++n) {
        if (count_rep[n] == 3) {
            if (terms_len < MAX_TERMS)
                terms[terms_len++] = (seq_t)n;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        build_counts();
        initialized = true;
    }
    if (n == 0 || n > terms_len)
        return 0;          /* out of range */
    return terms[n - 1];   /* 1‑based indexing */
}
