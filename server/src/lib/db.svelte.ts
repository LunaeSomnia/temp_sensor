import sqlite3 from "sqlite3";
import { open, Database, Statement } from "sqlite";
import type { TemperatureData } from "./model";

sqlite3.verbose();
let db: Database<sqlite3.Database, sqlite3.Statement> | undefined = undefined;
let insertStatement: Statement<sqlite3.Statement> | undefined = undefined;
let getAllStatement: Statement<sqlite3.Statement> | undefined = undefined;
let getLast20Statement: Statement<sqlite3.Statement> | undefined = undefined;

export async function initDb() {
    db = await open({
        filename: "./temp.db",
        driver: sqlite3.Database,
    });

    db.exec(`CREATE TABLE IF NOT EXISTS "temp_data" (
      "timestamp" DATETIME NOT NULL PRIMARY KEY,
      "data" REAL NOT NULL
    );`);

    insertStatement = await db.prepare(
        `INSERT INTO "temp_data" VALUES(?1, ?2)`,
    );
    getAllStatement = await db.prepare(`SELECT * FROM "temp_data"`);
    getLast20Statement = await db.prepare(`SELECT timestamp, data FROM temp_data
    ORDER BY timestamp DESC
    LIMIT 20`);
}

export async function closeDb() {
    if (!db) {
        console.error("Database not initialized");
        return false;
    }
    await db?.close();
}

export async function insertData(data: TemperatureData) {
    if (!db || !insertStatement) {
        console.error("Database not initialized");
        return false;
    }

    await insertStatement.bind({ 1: data.timestamp, 2: data.data });
    const result = await insertStatement.run();
    return result.lastID !== undefined;
}

export async function getLastData(): Promise<TemperatureData[]> {
    if (!db || !getLast20Statement) {
        console.error("Database not initialized");
        return [];
    }

    return (await getLast20Statement.all()) as TemperatureData[];
}

export async function getAllData(): Promise<TemperatureData[]> {
    if (!db || !getAllStatement) {
        console.error("Database not initialized");
        return [];
    }

    return (await getAllStatement.all()) as TemperatureData[];
}
