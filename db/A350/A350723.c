/*
 * A350723  Integer defect of n: a(n) = A005245(n) - A349983(n).
 * https://oeis.org/A350723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005245(n) - A349983(n); A005245 by DP search: a(1)=1,
 *            a(n)=min(a(i)+a(n-i), a(d)+a(n/d)); A349983(n) = 3 +
 *            floor(log_3(n)) + floor(log_3(n/2)) + floor(log_3(n/4)) for n >
 *            3, with values 1, 2, 3 for n = 1, 2, 3
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static int *A005245_tab = NULL;
static idx_t A005245_done = 0;
static idx_t A005245_cap = 0;

static seq_t A005245_a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n > A005245_done) {
        if (n + 1 > A005245_cap) {
            idx_t nc = A005245_cap ? A005245_cap : 64;
            while (nc < n + 1) nc *= 2;
            int *nt = (int *)realloc(A005245_tab, (size_t)nc * sizeof(int));
            if (!nt) return (seq_t)0;
            A005245_tab = nt;
            A005245_cap = nc;
        }
        int *t = A005245_tab;
        if (A005245_done == 0) {
            t[0] = 0;
            t[1] = 1;
            A005245_done = 1;
        }
        for (idx_t m = A005245_done + 1; m <= n; m++) {
            int best = t[m - 1] + 1;
            for (idx_t i = 2; i <= m / 2; i++) {
                int c = t[i] + t[m - i];
                if (c < best) best = c;
            }
            for (idx_t d = 2; d * d <= m; d++) {
                if (m % d == 0) {
                    int c = t[d] + t[m / d];
                    if (c < best) best = c;
                }
            }
            t[m] = best;
        }
        A005245_done = n;
    }
    return (seq_t)A005245_tab[n];
}

static const idx_t A349983_POW3[] = {
    1ULL, 3ULL, 9ULL, 27ULL, 81ULL, 243ULL, 729ULL, 2187ULL, 6561ULL,
    19683ULL, 59049ULL, 177147ULL, 531441ULL, 1594323ULL, 4782969ULL,
    14348907ULL, 43046721ULL, 129140163ULL, 387420489ULL, 1162261467ULL,
    3486784401ULL, 10460353203ULL, 31381059609ULL, 94143178827ULL,
    282429536481ULL, 847288609443ULL, 2541865828329ULL, 7625597484987ULL,
    22876792454961ULL, 68630377364883ULL, 205891132094649ULL,
    617673396283947ULL, 1853020188851841ULL, 5559060566557443ULL,
    16677181699666569ULL, 50031545098999707ULL, 150094635296999121ULL,
    450283905890997363ULL, 1350851717672992089ULL,
    4052555153018976267ULL, 12157665459056928801ULL
};

static idx_t A349983_floor_log3(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 0, high = 40;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (A349983_POW3[mid] <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static seq_t A349983_a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;
    idx_t k1 = A349983_floor_log3(n);
    idx_t k2 = A349983_floor_log3(n / 2);
    idx_t k3 = A349983_floor_log3(n / 4);
    seq_t result = 3;
    result += (seq_t)k1;
    result += (seq_t)k2;
    result += (seq_t)k3;
    return result;
}

seq_t a(idx_t n)
{
    seq_t x = A005245_a(n);
    seq_t y = A349983_a(n);
    return x - y;
}
