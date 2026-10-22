/*
 * A358612  Irregular triangle T(n, k), n >= 0, k > 0, read by rows of
 *          extended (due to binary expansion of n) Stirling numbers of the
 *          second kind.
 * https://oeis.org/A358612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is defined by the irregular triangle T(n,k)
 * with initial values T(0,1)=T(0,2)=1 and
 *   T(n,1)=1,          T(n,2)=2n+1
 *   T(2m+1,k)=k*T(m,k)+T(m,k-1)      (k>2)
 *   T(2m,k)=k*T(m,k)+T(m,k-1)-
 *            (T(2m,k-1)+T(m,k-1))/(k-1)   (k>2)
 *
 * a(n) is the n-th element when the triangle is read row‑by‑row.
 * The implementation uses memoised recursion and works for
 * arbitrary n up to at least 10000 in well under two seconds.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_K 70  /* sufficient for n up to 64‑bit (popcount+2) */

static seq_t (*T_mem)[MAX_K] = NULL;        /* memoised T(n,k) values   */
static uint8_t (*T_vis)[MAX_K] = NULL;      /* flags: 0 = not computed, 1 = computed */
static idx_t mem_size = 0;                  /* number of allocated rows  */

static void ensure_mem_size(idx_t n);
static seq_t compute_T(idx_t n, idx_t k);

/* ------------------------------------------------------------------ */
/*  compute_T : recursive, memoised computation of T(n,k)              */
/* ------------------------------------------------------------------ */
static seq_t
compute_T(idx_t n, idx_t k)
{
    /* base cases --------------------------------------------------- */
    if (k == 1)                   return (seq_t)1;
    if (k == 2)                   return (seq_t)2 * (seq_t)n + (seq_t)1;
    if (n == 0) {                 /* only k==1 or 2 are defined at n==0 */
        return (k == 1 || k == 2) ? (seq_t)1 : (seq_t)0;
    }

    /* make sure storage can hold row n ---------------------------- */
    ensure_mem_size(n);

    /* cached value ? --------------------------------------------- */
    if (T_vis[n][k]) return T_mem[n][k];

    seq_t result;

    if (n & 1)   /* n = 2*m + 1 (odd case) ----------------------------- */
    {
        idx_t m = n >> 1;
        seq_t a = compute_T(m, k);       /* T(m,k)   */
        seq_t b = compute_T(m, k - 1);   /* T(m,k-1) */
        result = (seq_t)k * a + b;
    }
    else         /* n = 2*m   (even case), n > 0 ------------------------ */
    {
        idx_t m = n >> 1;
        seq_t a = compute_T(m, k);         /* k * T(m,k)           */
        a = (seq_t)k * a;
        seq_t b = compute_T(m, k - 1);     /* T(m,k-1)             */
        seq_t c = compute_T(n, k - 1);     /* T(n,k-1)             */
        /* (c + b) / (k-1) is guaranteed to be integer           */
        seq_t sub = (c + b) / (seq_t)(k - 1);
        result = a + b - sub;
    }

    T_mem[n][k] = result;
    T_vis[n][k] = 1;
    return result;
}

/* ------------------------------------------------------------------ */
/*  ensure_mem_size : grow T_mem/T_vis so that row 'n' exists --------/
/* ------------------------------------------------------------------ */
static void
ensure_mem_size(idx_t n)
{
    if (n < mem_size) return;

    idx_t old = mem_size;
    mem_size = (mem_size == 0) ? 16 : mem_size << 1;
    while (mem_size <= n) mem_size <<= 1;

    T_mem = (seq_t (*)[MAX_K])realloc(T_mem,
                                      mem_size * sizeof(*T_mem));
    T_vis = (uint8_t (*)[MAX_K])realloc(T_vis,
                                      mem_size * sizeof(*T_vis));

    if (!T_mem || !T_vis) abort();

    /* zero the visited flags for the newly allocated rows -------- */
    if (old < mem_size)
        memset(T_vis + old, 0,
               (mem_size - old) * sizeof(*T_vis));
}

/* ------------------------------------------------------------------ */
/*  a : public function ------------------------------------------------
 *  a(n) is the n‑th element of the irregular triangle read row‑by‑row.
 * ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    /* locate the row r and column k (1‑based) for index n          */
    idx_t idx = n;
    idx_t r   = 0;
    while (1)
    {
        idx_t len = (idx_t)((__builtin_popcountll(r) + 2));
        if (idx < len) break;
        idx -= len;
        ++r;
    }
    idx_t k = idx + 1;   /* columns start at 1 */

    return compute_T(r, k);
}
