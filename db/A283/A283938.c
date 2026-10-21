/*
 * A283938  Interspersion of the signature sequence of phi^2, where phi = (1 +
 *          sqrt(5))/2 = golden ratio.
 * https://oeis.org/A283938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = rank of the pair (i,j) in the Beatty lattice of r = φ^2,
 *            where (i,j) is the pair appearing at position n in the
 *            anti‑diagonal enumeration
 * Range:     u8 n <= 105, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#define MAXK 2000   /* sufficient for n up to at least 10000 */

static const long double R = (3.0L + sqrtl(5.0L)) / 2.0L; /* φ^2 */
static uint64_t floor_r[MAXK + 1];
static bool init_done = false;

static void init_floor_r(void)
{
    if (!init_done) {
        for (int k = 0; k <= MAXK; ++k) {
            floor_r[k] = (uint64_t)(k * R);        /* floor(k * r) */
        }
        init_done = true;
    }
}

/* Floor of k * R for possibly negative integer k */
static inline int64_t floork(int k)
{
    if (k >= 0)
        return (int64_t)floor_r[k];
    /* floor(-k * R) = -(floor(k * R) + 1) ,  k>0  */
    int pos = -k;
    return -(int64_t)(floor_r[pos] + 1);
}

seq_t a(idx_t n)
{
    init_floor_r();                    /* prepare look‑up table */

    /* --- Determine (i,j) from anti‑diagonal order ------------------- */
    uint64_t N = (uint64_t)n;          /* n >= 1 */
    uint64_t d = 1;                    /* current diagonal (sum = d+1) */

    /* Find smallest d with d*(d+1)/2 >= N */
    while (d * (d + 1) / 2 < N) ++d;
    uint64_t prev = (d - 1) * d / 2;   /* pairs before diagonal d   */
    uint64_t offset = N - prev - 1;    /* 0‑based position in diag  */
    uint64_t i = offset + 1;
    uint64_t j = d - offset;           /* j index in current diagonal */

    /* --- Compute Jmax = largest j' with i + j'*r < i + j*r ---------- */
    uint64_t extra = (uint64_t)(floor((long double)(i - 1) / R)); /* floor((i-1)/r) */
    uint64_t Jmax = j + extra;               /* max j'                                  */

    /* --- Sum floor(k*r) for k = j-Jmax .. j-1 ----------------------- */
    int k_start = (int)j - (int)Jmax;
    int k_end   = (int)j - 1;
    int64_t sum = 0;
    for (int k = k_start; k <= k_end; ++k)
        sum += floork(k);

    /* --- Rank = Jmax * i + sum ------------------------------------- */
    uint64_t result = Jmax * i + sum;
    return (seq_t)result;
}
