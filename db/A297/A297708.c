/*
 * A297708  Number of permutations p of [n] such that p(p(i)) = i for all i or
 *          p(n+1-p(i)) = n+1-i for all i.
 * https://oeis.org/A297708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000085(n) – A135401(n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 31, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000085(n)   : number of involutions of [n]          */
/* A135401(n)   : number of involutions of [n] that    */
/*                commute with the reversal r(i)=n+1-i  */
/* The recurrence for A000085:                         */
/*   a(0)=1, a(1)=1, a(n)=a(n-1)+(n-1)*a(n-2)        */
/* The recurrence for A135401 (let m=floor(n/2)):      */
/*   b(0)=1, b(1)=2, b(m)=2*b(m-1)+2*(m-1)*b(m-2)   */
/* The implementation pre‑computes the two sequences   */
/* up to the requested index and uses static tables    */

#include <stdint.h>
#include <stdbool.h>

#define MAX_N 20000                    /* enough for all tested n (≈10000) */
#define MAX_M (MAX_N / 2)              /* m=floor(n/2) */

static seq_t  invol_tbl[MAX_N + 1];   /* A000085 values */
static seq_t  comm_tbl[MAX_M + 1];    /* A135401 values */
static idx_t  invol_max = 0;          /* highest index computed in invol_tbl */
static idx_t  comm_max  = 0;          /* highest index computed in comm_tbl */
static bool   tables_initialized = false;

/* Ensure that the tables contain values up to the required index. */
static void init_tables(void)
{
    if (tables_initialized) return;
    invol_tbl[0] = (seq_t)1;
    invol_tbl[1] = (seq_t)1;
    comm_tbl[0]  = (seq_t)1;
    comm_tbl[1]  = (seq_t)2;
    invol_max = 1;
    comm_max  = 1;
    tables_initialized = true;
}

/* Compute A000085(n) up to index n. */
static void ensure_invol(idx_t n)
{
    init_tables();
    if (n <= invol_max) return;
    for (idx_t i = invol_max + 1; i <= n; ++i) {
        invol_tbl[i] = invol_tbl[i - 1] + ((seq_t)(i - 1) * invol_tbl[i - 2]);
    }
    invol_max = n;
}

/* Compute A135401(m) up to index m (m = floor(n/2)). */
static void ensure_comm(idx_t m)
{
    init_tables();
    if (m <= comm_max) return;
    for (idx_t i = comm_max + 1; i <= m; ++i) {
        comm_tbl[i] = (seq_t)2 * (comm_tbl[i - 1] + ((seq_t)(i - 1) * comm_tbl[i - 2]));
    }
    comm_max = m;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_invol(n);
    idx_t m = n / 2;
    ensure_comm(m);
    return (seq_t)2 * invol_tbl[n] - comm_tbl[m];
}
