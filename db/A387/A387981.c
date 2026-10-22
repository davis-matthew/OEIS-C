/*
 * A387981  Positive integers m such that 2*m is not the sum of two
 *          horizontal, vertical or diagonal neighboring primes in a square
 *          spiral with odd primes.
 * https://oeis.org/A387981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer m such that * 2*m is NOT equal
 *            to the sum of two adjacent odd primes * (horizontal, vertical or
 *            diagonal) in the square spiral * obtained by placing successive
 *            odd primes in the usual * clockwise spiral order. * This
 *            implementation constructs the spiral for all od...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_prime_spiral, build_spiral_grid, list_odd_primes,
 *            sieve_bool
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/build_prime_spiral.h"
#include "../utils/build_spiral_grid.h"
#include "../utils/list_odd_primes.h"
#include "../utils/sieve_bool.h"

/* Size limits for precomputation */
#define MAX_M          40000              /* maximum m we are ready to answer */
#define MAX_PRIME      (2 * MAX_M)        /* primes up to this value are needed */
#define GRID_GROWTH    2
#define INITIAL_CAP    64

/* Main function that returns the nth term. */
seq_t a(idx_t n)
{
    /* static storage for sequence terms and intermediate data */
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static size_t terms_cap = 0;
    static bool data_ready = false;

    /* --- First call: precompute all data --- */
    if (!data_ready)
    {
        /* 1. Prime sieve up to MAX_PRIME */
        unsigned char *is_prime = (unsigned char *)malloc((MAX_PRIME + 1) * sizeof(unsigned char));
        sieve_bool(is_prime, MAX_PRIME);

        /* 2. List all odd primes */
        uint32_t *primes = NULL;
        size_t prime_cnt = 0;
        list_odd_primes(is_prime, MAX_PRIME, &primes, &prime_cnt);

        /* 3. Build coordinate grid */
        int offset = 200;  /* sufficient for prime_cnt ~ 7000 */
        int grid_size = offset * 2 + 1;
        int *grid = (int *)calloc(grid_size * grid_size, sizeof(int));
        if (!grid) { free(is_prime); free(primes); return 0; }
        build_spiral_grid(primes, prime_cnt, grid, offset, grid_size);

        /* 4. Forbidden sums up to 2*MAX_M */
        size_t forbidden_size = 2 * MAX_M + 1;
        bool *forbidden = (bool *)calloc(forbidden_size, sizeof(bool));

        /* Neighbor offsets */
        int ndx[] = {1, -1, 0, 0, 1, 1, -1, -1};
        int ndy[] = {0, 0, 1, -1, 1, -1, 1, -1};

        for (int y = 0; y < grid_size; ++y)
            for (int x = 0; x < grid_size; ++x)
            {
                int p = grid[x + y * grid_size];
                if (!p) continue;
                for (int k = 0; k < 8; ++k)
                {
                    int nx = x + ndx[k];
                    int ny = y + ndy[k];
                    if (nx < 0 || ny < 0 || nx >= grid_size || ny >= grid_size) continue;
                    int q = grid[nx + ny * grid_size];
                    if (!q) continue;
                    size_t s = (size_t)p + (size_t)q;
                    if (s < forbidden_size) forbidden[s] = true;
                }
            }

        /* 5. Build sequence up to needed size (up to 20000 terms) */
        const size_t MAX_TERMS = 20000;
        terms = (seq_t *)malloc(MAX_TERMS * sizeof(seq_t));
        terms_cap = MAX_TERMS;
        terms_size = 0;

        seq_t m = 1;
        while (terms_size < MAX_TERMS && (size_t)(m * 2) < forbidden_size)
        {
            size_t s = (size_t)(m * 2) ;
            if (!forbidden[s])
            {
                terms[terms_size++] = m;
            }
            ++m;
        }

        /* Free temporary data */
        free(is_prime);
        free(primes);
        free(grid);
        free(forbidden);
        data_ready = true;
    }

    /* --- Return requested term --- */
    if (n == 0 || (size_t)n > terms_size)
        return 0;  /* undefined for n beyond precomputed range */
    return terms[n - 1];
}
