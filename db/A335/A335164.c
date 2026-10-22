/*
 * A335164  Nim n-th power of 5.
 * https://oeis.org/A335164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A335164 – Nim n‑th power of 5
   Order:  a(n+15) = a(n)
   Computation:  a(0)=1, a(n+1)=a(n)*5   (nimber multiplication)
   The multiplication table for nimbers 0…15 is precomputed once
   and reused; powers are then derived by repeated table lookup. */

#include <stdbool.h>

/* Nimber multiplication table for nimbers 0…15 */
static seq_t nim_mul_table[16][16];
static bool nim_mul_init[16][16] = {{false}};

/* Pre‑computed powers of 5; period 15 */
static seq_t powers[15];
static bool ready = false;

/* Recursive nimber multiplication for 0 ≤ a,b ≤ 15  */
static seq_t nim_mul_rec(seq_t a, seq_t b)
{
    /* Base cases are handled in init_nim; return values only for recursion */
    if (a == 0 || b == 0) return 0;
    if (a == 1) return b;
    if (b == 1) return a;
    if (nim_mul_init[(int)a][(int)b]) return nim_mul_table[(int)a][(int)b];

    bool seen[32] = {false};
    for (int ai = 0; ai < (int)a; ++ai) {
        seq_t xa = nim_mul_rec((seq_t)ai, b);
        for (int bi = 0; bi < (int)b; ++bi) {
            seq_t xb = nim_mul_rec(a, (seq_t)bi);
            seq_t xab = nim_mul_rec((seq_t)ai, (seq_t)bi);
            int val = (int)(xa ^ xb ^ xab);
            if (val < 32) seen[val] = true;
        }
    }
    int mex = 0;
    while (mex < 32 && seen[mex]) ++mex;

    nim_mul_table[(int)a][(int)b] = (seq_t)mex;
    nim_mul_table[(int)b][(int)a] = (seq_t)mex;
    nim_mul_init[(int)a][(int)b] = nim_mul_init[(int)b][(int)a] = true;

    return (seq_t)mex;
}

/* One‑time initialization: fill base cases, compute table, and powers. */
static void init_nim(void)
{
    if (ready) return;
    /* Base cases: multiplication by 0 or 1 */
    for (int i = 0; i <= 15; ++i) {
        nim_mul_table[0][i] = nim_mul_table[i][0] = 0;
        nim_mul_init[0][i] = nim_mul_init[i][0] = true;

        nim_mul_table[1][i] = nim_mul_table[i][1] = (seq_t)i;
        nim_mul_init[1][i] = nim_mul_init[i][1] = true;
    }

    /* Compute the full multiplication table for the remaining pairs */
    for (int a = 0; a <= 15; ++a) {
        for (int b = 0; b <= 15; ++b) {
            if (!nim_mul_init[a][b]) nim_mul_rec((seq_t)a, (seq_t)b);
        }
    }

    /* Pre‑compute the 15‑term period of powers of 5 */
    powers[0] = 1;                           /* 5^0 */
    for (int i = 1; i < 15; ++i) {
        powers[i] = nim_mul_table[(int)powers[i-1]][5];
    }

    ready = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    init_nim();
    return powers[(unsigned)(n % 15)];
}
