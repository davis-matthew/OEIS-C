/*
 * A376508  Natural numbers whose iterated squaring modulo 100 eventually
 *          enters the 4-cycle 16, 56, 36, 96.
 * https://oeis.org/A376508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth positive integer such that repeated squaring
 * modulo 100 eventually enters the 4‑cycle {16,56,36,96}.
 * The function computes the nth term by precomputing all residues modulo 100
 * that can reach the cycle under repeated squaring, then scanning natural
 * numbers until the nth accepted number is found.
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static bool good_res[100];
static bool initialized = false;

/* Compute all residues modulo 100 that eventually reach the 4‑cycle
 * 16, 56, 36, 96 under repeated squaring. */
static void compute_good_residues(void)
{
    bool visited[100] = {0};
    int queue[100];
    int head = 0, tail = 0;

    /* start with the cycle residues */
    int cycle[4] = {16, 56, 36, 96};
    for (int i = 0; i < 4; ++i) {
        int y = cycle[i];
        if (!visited[y]) {
            visited[y] = true;
            queue[tail++] = y;
        }
    }

    /* propagate backwards: for each residue y, find all x with x^2 ≡ y (mod 100) */
    while (head < tail) {
        int y = queue[head++];
        for (int x = 0; x < 100; ++x) {
            if ((x * x) % 100 == y && !visited[x]) {
                visited[x] = true;
                queue[tail++] = x;
            }
        }
    }

    memcpy(good_res, visited, 100);
}

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_good_residues();
        initialized = true;
    }

    /* Find the nth natural number whose residue mod 100 is good. */
    idx_t count = 0;
    idx_t num = 1;
    seq_t result = 0;

    while (count < n) {
        if (good_res[num % 100]) {
            ++count;
            if (count == n)
                result = (seq_t)num;
        }
        ++num;
    }

    return result;
}
