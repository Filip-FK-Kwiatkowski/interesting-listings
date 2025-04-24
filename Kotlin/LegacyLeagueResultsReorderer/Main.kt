package org.example

import java.io.File

class PlayerRow(
    val nameAndSurname: String,
    var totalPoints: Int,
    val points: MutableList<Int?> = mutableListOf(),
) : Comparable<PlayerRow> {
    fun recalculateTotalPoints() {
        totalPoints = points.filterNotNull().sum()
    }

    override fun compareTo(other: PlayerRow): Int {
        return other.totalPoints.compareTo(totalPoints)
    }

    override fun toString(): String {
        val formattedPoints = points.joinToString(separator = " | ") { it?.toString().orEmpty() }

        return "| $nameAndSurname | $$ $totalPoints $$ | $formattedPoints |"
    }

    companion object {
        fun fromLine(line: String): PlayerRow {
            val rawPoints = line.split("|").drop(4).dropLast(1)

            val nameAndSurname = line.split("|")[2].trim()
            val points = rawPoints.map { it.trim().toIntOrNull() }

            val result = PlayerRow(nameAndSurname, 0, points.toMutableList())
            result.recalculateTotalPoints()
            return result
        }
    }
}

fun main() {
    val path = "path/to/your/file.md"
    val playerRows = File(path).useLines { lines ->
        lines
            .filter { it.startsWith('|') }
            .drop(2)
            .map(PlayerRow::fromLine)
            .toMutableList()
    }

    playerRows.sort()

    for ((index, player) in playerRows.withIndex()) {
        println("| $$ ${index + 1}. $$ $player")
    }
}
